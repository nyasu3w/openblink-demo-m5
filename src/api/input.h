/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file input.h
 * @brief API interface for input handling in mruby/c
 *
 * Defines the interface for the Input class in mruby/c, which provides
 * methods for checking button states on the M5Stack hardware.
 */
#ifndef API_INPUT_H
#define API_INPUT_H

#include "../lib/fn.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines the Input class and methods for mruby/c
 *
 * Creates the Input class and registers the pressed? and released? methods
 * which allow Ruby code to check button states.
 *
 * @return kSuccess always
 */
fn_t api_input_define(void);

#ifdef __cplusplus
}
#endif

#endif
