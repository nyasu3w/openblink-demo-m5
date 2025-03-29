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
#include "../app/configstore.h"

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
  ble_init();
  config_data_load();   // this should be placed after ble_init() for flash init
//  config_data_debug();

  gpio_num_t rgb_led_pin = (gpio_num_t)M5.getPin(m5::pin_name_t::rgb_led);
  uint8_t size;
  if(M5.getBoard()==m5::board_t::board_M5AtomMatrix) { size=25; }
  if (rgb_led_pin < 0) {  // workaround for rgb_led definition issue
    switch(M5.getBoard()){
      case m5::board_t::board_M5Dial:
      case m5::board_t::board_M5DinMeter:
      case m5::board_t::board_M5AirQ:
      case m5::board_t::board_M5StampPLC:
          rgb_led_pin = GPIO_NUM_21;
          break;
      default:
        break;
    }
  }
  // configure according to configstore
  struct config_item_data* data; 
  data = config_data_get("led_pin");
  if(data!= NULL) {
    rgb_led_pin = *(gpio_num_t*)data->data;
  } else {
  printf("led_pin not found\n");
  }
  data = config_data_get("led_num");
  if(data!= NULL) {
    printf("led_num found\n");
    size = *(uint8_t*)data->data;
  } else {
    size = 1;
  }
  printf("INITIAL DATA:  led_pin=%d, led_num=%d\n", rgb_led_pin, size);

  if (rgb_led_pin >= 0) {
    drv_led_init(rgb_led_pin, size);
  }
  return kSuccess;
}