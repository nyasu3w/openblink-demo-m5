/**
 * @file led.c
 * @brief LED driver implementation
 *
 * Implements functions for controlling WS2812 RGB LEDs using the ESP32 RMT
 * peripheral. Based on Espressif's LED strip example.
 */
#include "led.h"

#include "../lib/fn.h"

/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include <math.h>
#include <string.h>

#include "../lib/fn.h"
#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define RMT_LED_STRIP_RESOLUTION_HZ 40000000

static uint8_t led_strip_pixels_size = 0;
static uint8_t *led_strip_pixels = NULL;
static rmt_encoder_handle_t simple_encoder = NULL;
static rmt_channel_handle_t led_chan = NULL;

static const rmt_symbol_word_t ws2812_zero = {
    .level0 = 1,
    .duration0 = 0.3 * RMT_LED_STRIP_RESOLUTION_HZ / 1000000,  // T0H=0.3us
    .level1 = 0,
    .duration1 = 0.9 * RMT_LED_STRIP_RESOLUTION_HZ / 1000000,  // T0L=0.9us
};

static const rmt_symbol_word_t ws2812_one = {
    .level0 = 1,
    .duration0 = 0.9 * RMT_LED_STRIP_RESOLUTION_HZ / 1000000,  // T1H=0.9us
    .level1 = 0,
    .duration1 = 0.9 * RMT_LED_STRIP_RESOLUTION_HZ / 1000000,  // T1L=0.9us
};

// reset defaults to 50uS
static const rmt_symbol_word_t ws2812_reset = {
    .level0 = 0,
    .duration0 = RMT_LED_STRIP_RESOLUTION_HZ / 1000000 * 150,
    .level1 = 0,
    .duration1 = RMT_LED_STRIP_RESOLUTION_HZ / 1000000 * 150,
};

/**
 * @brief RMT encoder callback for WS2812 LED protocol
 *
 * Encodes byte data into RMT symbols according to the WS2812 protocol.
 *
 * @param data Pointer to the data to encode
 * @param data_size Size of the data in bytes
 * @param symbols_written Number of symbols already written
 * @param symbols_free Number of free symbols in the buffer
 * @param symbols Pointer to the symbol buffer
 * @param done Pointer to a flag indicating if encoding is complete
 * @param arg User argument (unused)
 * @return Number of symbols written
 */
static size_t encoder_callback(const void *data, size_t data_size,
                               size_t symbols_written, size_t symbols_free,
                               rmt_symbol_word_t *symbols, bool *done,
                               void *arg) {
  // We need a minimum of 8 symbol spaces to encode a byte. We only
  // need one to encode a reset, but it's simpler to simply demand that
  // there are 8 symbol spaces free to write anything.
  if (symbols_free < 8) {
    return 0;
  }

  // We can calculate where in the data we are from the symbol pos.
  // Alternatively, we could use some counter referenced by the arg
  // parameter to keep track of this.
  size_t data_pos = symbols_written / 8;
  uint8_t *data_bytes = (uint8_t *)data;
  if (data_pos < data_size) {
    // Encode a byte
    size_t symbol_pos = 0;
    for (int bitmask = 0x80; bitmask != 0; bitmask >>= 1) {
      if (data_bytes[data_pos] & bitmask) {
        symbols[symbol_pos++] = ws2812_one;
      } else {
        symbols[symbol_pos++] = ws2812_zero;
      }
    }
    // We're done; we should have written 8 symbols.
    return symbol_pos;
  } else {
    // All bytes already are encoded.
    // Encode the reset, and we're done.
    symbols[0] = ws2812_reset;
    *done = 1;  // Indicate end of the transaction.
    return 1;   // we only wrote one symbol
  }
}

/**
 * @brief Initializes the LED driver
 *
 * Sets up the RMT peripheral for controlling WS2812 RGB LEDs.
 *
 * @return kSuccess on successful initialization
 */
fn_t drv_led_init(gpio_num_t pin_num, uint8_t size) {
  if (led_strip_pixels != NULL) {
    return kFailure;
  }
  led_strip_pixels_size = size;
  led_strip_pixels = (uint8_t *)malloc(led_strip_pixels_size * 3);
  memset(led_strip_pixels, 0, led_strip_pixels_size * 3);
  gpio_reset_pin(pin_num);
  rmt_tx_channel_config_t tx_chan_config = {
      .clk_src = RMT_CLK_SRC_DEFAULT,  // select source clock
      .gpio_num = pin_num,
      .mem_block_symbols =
          64,  // increase the block size can make the LED less flickering
      .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
      .trans_queue_depth = 4,  // set the number of transactions that can be
                               // pending in the background
  };
  ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

  const rmt_simple_encoder_config_t simple_encoder_cfg = {
      .callback = encoder_callback
      // Note we don't set min_chunk_size here as the default of 64 is good
      // enough.
  };
  ESP_ERROR_CHECK(rmt_new_simple_encoder(&simple_encoder_cfg, &simple_encoder));

  ESP_ERROR_CHECK(rmt_enable(led_chan));

  return kSuccess;
}

/**
 * @brief Sets the RGB LED color
 *
 * @param kNum LED index (0-based)
 * @param kRed Red component (0-255)
 * @param kGreen Green component (0-255)
 * @param kBlue Blue component (0-255)
 * @return kSuccess on successful operation, kFailure if LED index is invalid
 */
fn_t drv_led_set(const uint8_t kNum, const uint8_t kRed, const uint8_t kGreen,
                 const uint8_t kBlue) {
  if (kNum >= led_strip_pixels_size) {
    return kFailure;
  }

  rmt_transmit_config_t tx_config = {
      .loop_count = 0,  // no transfer loop
  };

  led_strip_pixels[kNum * 3 + 0] = kGreen;
  led_strip_pixels[kNum * 3 + 1] = kRed;
  led_strip_pixels[kNum * 3 + 2] = kBlue;

  // Flush RGB values to LEDs
  ESP_ERROR_CHECK(rmt_transmit(led_chan, simple_encoder, led_strip_pixels,
                               led_strip_pixels_size*3, &tx_config));
  ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));

  return kSuccess;
}
