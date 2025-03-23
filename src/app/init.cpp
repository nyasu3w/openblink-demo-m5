/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file init.cpp
 * @brief Application initialization implementation
 *
 * Implements the initialization of application components including
 * M5Stack hardware, LED driver, and BLE functionality.
 */
#include "init.h"

#include <M5Unified.h>

#include "../lib/fn.h"
extern "C" {
#include "../drv/ble.h"
#include "../drv/led.h"
}

/**
 * @brief Initializes the application components
 *
 * Initializes the M5Stack hardware, LED driver, and BLE functionality.
 *
 * @return kSuccess always
 */
fn_t app_init(void) {
  M5.begin();
  gpio_num_t rgb_led_pin = (gpio_num_t)M5.getPin(m5::pin_name_t::rgb_led);
  uint8_t size = 1;
  if(M5.getBoard()==m5::board_t::board_M5AtomMatrix) { size=25; }

  if (rgb_led_pin < 0) {  // workaround for rgb_led definition issue
    switch(M5.getBoard()){
        case m5::board_t::board_M5Dial:
            rgb_led_pin = (gpio_num_t)M5.getPin(m5::pin_name_t::port_a_pin2);
            size=70;
            break;
        case m5::board_t::board_M5DinMeter:
        case m5::board_t::board_M5AirQ:
        case m5::board_t::board_M5StampPLC:
            rgb_led_pin = GPIO_NUM_21;
            break;
        default:
          break;
    }
  }

  if (rgb_led_pin >= 0) {
    drv_led_init(rgb_led_pin, size);
  }
  ble_init();
  return kSuccess;
}