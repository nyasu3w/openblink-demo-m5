/*
 * SPDX-License-Identifier: BSD-3-Clause
 * SPDX-FileCopyrightText: Copyright (c) 2025 ViXion Inc. All Rights Reserved.
 */
/**
 * @file blink.c
 * @brief Implementation of application-level Blink functionality
 *
 * Implements functions for loading, storing, and managing bytecode
 * in non-volatile storage (NVS) for the Blink feature.
 */
#include "blink.h"

#include <stddef.h>
#include <stdint.h>

#include "../lib/fn.h"
#include "nvs.h"
#include "nvs_flash.h"

/**
 * @brief Loads bytecode from NVS storage
 *
 * Retrieves bytecode from the "slot2" key in the "openblink" namespace
 * of non-volatile storage.
 *
 * @param data Pointer to buffer where bytecode will be loaded
 * @param kLength Maximum size of the buffer
 * @return Size of loaded bytecode, or 0 if loading failed
 */
size_t blink_load(uint8_t *const data, const size_t kLength) {
  nvs_handle_t handle;
  size_t length = kLength;
  if (kLength < blink_get_data_length()) {
    return 0;
  }
  if (ESP_OK != nvs_open("openblink", NVS_READONLY, &handle)) {
    return 0;
  }
  if (ESP_OK != nvs_get_blob(handle, "slot2", data, &length)) {
    nvs_close(handle);
    return 0;
  }
  nvs_close(handle);
  return length;
}

/**
 * @brief Stores bytecode to NVS storage
 *
 * Saves bytecode to the "slot2" key in the "openblink" namespace
 * of non-volatile storage.
 *
 * @param kData Pointer to bytecode data to store
 * @param kLength Size of bytecode data
 * @return Size of stored bytecode, or 0 if storing failed
 */
size_t blink_store(const uint8_t *const kData, const size_t kLength) {
  nvs_handle_t handle;
  if (ESP_OK != nvs_open("openblink", NVS_READWRITE, &handle)) {
    return 0;
  }
  if (ESP_OK != nvs_set_blob(handle, "slot2", kData, kLength)) {
    nvs_close(handle);
    return 0;
  }
  if (ESP_OK != nvs_commit(handle)) {
    nvs_close(handle);
    return 0;
  }
  nvs_close(handle);
  return kLength;
}

/**
 * @brief Gets the length of bytecode stored in NVS
 *
 * Retrieves the size of bytecode stored in the "slot2" key
 * without actually loading the data.
 *
 * @return Size of stored bytecode, or 0 if no bytecode is stored
 */
size_t blink_get_data_length(void) {
  nvs_handle_t handle;
  size_t length = 0;
  if (ESP_OK != nvs_open("openblink", NVS_READONLY, &handle)) {
    return 0;
  }
  if (ESP_OK != nvs_get_blob(handle, "slot2", 0, &length)) {
    nvs_close(handle);
    return 0;
  }
  nvs_close(handle);
  return length;
}

/**
 * @brief Deletes bytecode from NVS storage
 *
 * Removes the "slot2" key from the "openblink" namespace
 * in non-volatile storage.
 *
 * @return 0 on success, -1 on failure
 */
int blink_delete(void) {
  nvs_handle_t handle;
  if (ESP_OK != nvs_open("openblink", NVS_READWRITE, &handle)) {
    nvs_close(handle);
    return -1;
  }
  if (ESP_OK != nvs_erase_key(handle, "slot2")) {
    nvs_close(handle);
    return -1;
  }
  if (ESP_OK != nvs_commit(handle)) {
    nvs_close(handle);
    return -1;
  }
  nvs_close(handle);
  return 0;
}
