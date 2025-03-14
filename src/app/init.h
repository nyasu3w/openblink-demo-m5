/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file init.h
 * @brief Application initialization interface
 *
 * Defines the interface for initializing the application components.
 */
#ifndef APP_INIT_H
#define APP_INIT_H

#include "../lib/fn.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the application components
 *
 * Initializes the M5Stack hardware, LED driver, and BLE functionality.
 *
 * @return kSuccess always
 */
fn_t app_init(void);

#ifdef __cplusplus
}
#endif

#endif
