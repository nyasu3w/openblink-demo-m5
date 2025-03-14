/*
 * Copyright (c) 2017 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file crc16_sw.c
 * @brief Software implementation of CRC-16 algorithm
 *
 * Implements the CRC-16 algorithm with reflection for data integrity
 * verification.
 */
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Calculates CRC-16 with input and output reflection
 *
 * Computes CRC-16 by processing each byte of input data with
 * the specified polynomial and seed value.
 *
 * @param poly The reflected polynomial to use (e.g., 0xA001 for CRC-16-MODBUS)
 * @param seed Initial value for the CRC computation
 * @param src Pointer to the input data buffer
 * @param len Length of the input data in bytes
 * @return The computed CRC-16 value
 */
uint16_t crc16_reflect(uint16_t poly, uint16_t seed, const uint8_t *src,
                       size_t len) {
  uint16_t crc = seed;
  size_t i, j;

  for (i = 0; i < len; i++) {
    crc ^= (uint16_t)src[i];

    for (j = 0; j < 8; j++) {
      if (crc & 0x0001UL) {
        crc = (crc >> 1U) ^ poly;
      } else {
        crc = crc >> 1U;
      }
    }
  }

  return crc;
}
