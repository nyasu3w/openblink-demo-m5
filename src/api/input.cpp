/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file input.cpp
 * @brief Implementation of Input API for mruby/c
 *
 * Implements the Input class and its methods for the mruby/c VM,
 * providing functionality to check button states on the M5Stack hardware.
 */
#include "input.h"

#include <M5Unified.h>
#include <stdbool.h>

#include "../../mrubyc/src/mrubyc.h"
#include "../lib/fn.h"

/**
 * @brief Forward declarations for the mruby/c method implementations
 *
 * @param vm Pointer to the mruby/c VM
 * @param v Pointer to the method arguments
 * @param argc Number of arguments
 */
static void c_get_sw_pressed(mrb_vm *vm, mrb_value *v, int argc);
static void c_get_sw_released(mrb_vm *vm, mrb_value *v, int argc);

/**
 * @brief Defines the Input class and methods for mruby/c
 *
 * Creates the Input class and registers the pressed? and released? methods
 * which allow Ruby code to check button states.
 *
 * @return kSuccess always
 */
fn_t api_input_define(void) {
  mrb_class *class_input;
  class_input = mrbc_define_class(0, "Input", mrbc_class_object);
  mrbc_define_method(0, class_input, "pressed?", c_get_sw_pressed);
  mrbc_define_method(0, class_input, "released?", c_get_sw_released);
  return kSuccess;
}

/**
 * @brief Implementation of the pressed? method for the Input class
 *
 * Checks if the button A is currently pressed.
 *
 * @param vm Pointer to the mruby/c VM
 * @param v Pointer to the method arguments
 * @param argc Number of arguments
 */
static void c_get_sw_pressed(mrb_vm *vm, mrb_value *v, int argc) {
  M5.update();
  if (true == M5.BtnA.isPressed()) {
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

/**
 * @brief Implementation of the released? method for the Input class
 *
 * Checks if the button A was just released.
 *
 * @param vm Pointer to the mruby/c VM
 * @param v Pointer to the method arguments
 * @param argc Number of arguments
 */
static void c_get_sw_released(mrb_vm *vm, mrb_value *v, int argc) {
  M5.update();
  if (true == M5.BtnA.isReleased()) {
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}
