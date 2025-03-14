/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file blink.h
 * @brief Application-level Blink functionality
 *
 * Provides functions for loading, storing, and managing bytecode
 * for the Blink feature.
 */
#ifndef APP_BLINK_H
#define APP_BLINK_H

#include <stddef.h>
#include <stdint.h>

#define BLINK_MAX_BYTECODE_SIZE (15 * 1024)

/**
 * @brief Loads bytecode from NVS storage
 *
 * @param data Pointer to buffer where bytecode will be loaded
 * @param kLength Maximum size of the buffer
 * @return Size of loaded bytecode, or 0 if loading failed
 */
size_t blink_load(uint8_t *const data, const size_t kLength);

/**
 * @brief Stores bytecode to NVS storage
 *
 * @param kData Pointer to bytecode data to store
 * @param kLength Size of bytecode data
 * @return Size of stored bytecode, or 0 if storing failed
 */
size_t blink_store(const uint8_t *const kData, const size_t kLength);

/**
 * @brief Gets the length of bytecode stored in NVS
 *
 * @return Size of stored bytecode, or 0 if no bytecode is stored
 */
size_t blink_get_data_length(void);

/**
 * @brief Deletes bytecode from NVS storage
 *
 * @return 0 on success, -1 on failure
 */
int blink_delete(void);

#endif
