/**
 * @file ble.h
 * @brief Bluetooth Low Energy (BLE) driver interface
 *
 * Provides functions and variables for initializing and managing
 * BLE communication on the M5Stack hardware.
 */
#ifndef DRV_BLE_H
#define DRV_BLE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initializes the BLE subsystem
 *
 * Sets up the NimBLE stack, configures services, and starts advertising.
 */
void ble_init(void);

/**
 * @brief Handle for the heart rate measurement characteristic
 *
 * Used for sending notifications to connected clients.
 */
extern uint16_t hrs_hrm_handle;

/**
 * @brief Flag indicating if notifications are enabled
 *
 * True if a client has enabled notifications, false otherwise.
 */
extern bool notify_state;

/**
 * @brief Handle for the current BLE connection
 *
 * Stores the connection handle when a client is connected.
 */
extern uint16_t conn_handle;

#endif
