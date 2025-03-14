/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file led.h
 * @brief API interface for LED control in mruby/c
 *
 * Defines the interface for the LED class in mruby/c, which provides
 * methods for controlling the RGB LED on the M5Stack hardware.
 */
#ifndef API_LED_H
#define API_LED_H

#include "../lib/fn.h"

/**
 * @brief Defines the LED class and methods for mruby/c
 *
 * Creates the LED class and registers the set method
 * which allows Ruby code to control the RGB LED.
 *
 * @return kSuccess always
 */
fn_t api_led_define(void);

#endif
