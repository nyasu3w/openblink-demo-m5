/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file led.c
 * @brief Implementation of LED API for mruby/c
 *
 * Implements the LED class and its methods for the mruby/c VM,
 * providing functionality to control the RGB LED on the M5Stack hardware.
 */
#include "led.h"

#include <stdbool.h>
#include <stdint.h>

#include "../../.pio/libdeps/m5stack-stamps3/mrubyc/src/mrubyc.h"
#include "../drv/led.h"
#include "../lib/fn.h"

/**
 * @brief Forward declaration for the mruby/c method implementation
 *
 * @param vm Pointer to the mruby/c VM
 * @param v Pointer to the method arguments
 * @param argc Number of arguments
 */
static void c_set_led(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Defines the LED class and methods for mruby/c
 *
 * Creates the LED class and registers the set method
 * which allows Ruby code to control the RGB LED.
 *
 * @return kSuccess always
 */
fn_t api_led_define(void) {
  mrb_class *class_led;
  class_led = mrbc_define_class(0, "LED", mrbc_class_object);
  mrbc_define_method(0, class_led, "set", c_set_led);
  return kSuccess;
}

/**
 * @brief Implementation of the set method for the LED class
 *
 * Sets the RGB LED color based on the provided RGB array.
 * The array should contain 3 values for red, green, and blue (0-255).
 *
 * @param vm Pointer to the mruby/c VM
 * @param v Pointer to the method arguments
 * @param argc Number of arguments
 */
static void c_set_led(mrb_vm *vm, mrb_value *v, int argc) {
  uint8_t rgb[3] = {0};
  uint8_t num = 0;
  SET_FALSE_RETURN();

  do {
    if (MRBC_TT_ARRAY == v[1].tt) {
      if (3 != v[1].array->n_stored) break;
      for (size_t i = 0; i < 3; i++) {
        rgb[i] = (uint8_t)v[1].array->data[i].i;
      }
    } else {
      break;
    }
  } while (0);

  if (2 <= argc && MRBC_TT_INTEGER == v[2].tt) {
    num = (uint8_t)v[2].i;
  }

  if (kSuccess == drv_led_set(num, rgb[0], rgb[1], rgb[2])) {
    SET_TRUE_RETURN();
  }
}
