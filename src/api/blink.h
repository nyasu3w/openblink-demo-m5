/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file blink.h
 * @brief API interface for Blink functionality in mruby/c
 *
 * Defines the interface for the Blink class in mruby/c, which provides
 * methods for checking reload requests.
 */
#ifndef API_BLINK_H
#define API_BLINK_H

#include "../lib/fn.h"

/**
 * @brief Defines the Blink class and methods for mruby/c
 *
 * Creates the Blink class and registers the req_reload? method
 * which allows Ruby code to check if a reload has been requested.
 *
 * @return kSuccess always
 */
fn_t api_blink_define(void);

#endif
