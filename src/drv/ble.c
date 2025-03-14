/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/**
 * @file ble.c
 * @brief Bluetooth Low Energy (BLE) driver implementation
 *
 * Implements BLE functionality using the NimBLE stack, including
 * initialization, advertising, and connection management.
 */
// #include "esp_log.h"
#include "freertos/FreeRTOSConfig.h"
#include "nvs_flash.h"
/* BLE */
#include "ble_blink.h"
#include "console/console.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"

bool notify_state = false;
uint16_t conn_handle;
static const char *device_name = "OpenBlink_ESP32";
static int blehr_gap_event(struct ble_gap_event *event, void *arg);
static uint8_t blehr_addr_type;
struct ble_hs_cfg;
uint16_t hrs_hrm_handle;

/*
 * Enables advertising with parameters:
 *     o General discoverable mode
 *     o Undirected connectable mode
 */
/**
 * @brief Starts BLE advertising
 *
 * Configures and starts BLE advertising with general discoverable
 * and undirected connectable modes.
 */
static void blehr_advertise(void) {
  struct ble_gap_adv_params adv_params;
  struct ble_hs_adv_fields fields;
  int rc;

  /*
   *  Set the advertisement data included in our advertisements:
   *     o Flags (indicates advertisement type and other general info)
   *     o Advertising tx power
   *     o Device name
   */
  memset(&fields, 0, sizeof(fields));

  /*
   * Advertise two flags:
   *      o Discoverability in forthcoming advertisement (general)
   *      o BLE-only (BR/EDR unsupported)
   */
  fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;

  /*
   * Indicate that the TX power level field should be included; have the
   * stack fill this value automatically.  This is done by assigning the
   * special value BLE_HS_ADV_TX_PWR_LVL_AUTO.
   */
  fields.tx_pwr_lvl_is_present = 1;
  fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

  fields.name = (uint8_t *)device_name;
  fields.name_len = strlen(device_name);
  fields.name_is_complete = 1;

  rc = ble_gap_adv_set_fields(&fields);
  if (rc != 0) {
    //    MODLOG_DFLT(ERROR, "error setting advertisement data; rc=%d\n", rc);
    return;
  }

  /* Begin advertising */
  memset(&adv_params, 0, sizeof(adv_params));
  adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
  adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
  rc = ble_gap_adv_start(blehr_addr_type, NULL, BLE_HS_FOREVER, &adv_params,
                         blehr_gap_event, NULL);
  if (rc != 0) {
    //    MODLOG_DFLT(ERROR, "error enabling advertisement; rc=%d\n", rc);
    return;
  }
}

/**
 * @brief GAP event handler
 *
 * Handles BLE GAP events such as connection establishment,
 * disconnection, and subscription changes.
 *
 * @param event Pointer to the GAP event structure
 * @param arg User argument (unused)
 * @return 0 always
 */
static int blehr_gap_event(struct ble_gap_event *event, void *arg) {
  switch (event->type) {
    case BLE_GAP_EVENT_LINK_ESTAB:
      /* A new connection was established or a connection attempt failed */
      //      MODLOG_DFLT(INFO, "connection %s; status=%d\n",
      //                  event->link_estab.status == 0 ? "established" :
      //                  "failed", event->link_estab.status);

      if (event->link_estab.status != 0) {
        /* Connection failed; resume advertising */
        blehr_advertise();
      }
      conn_handle = event->link_estab.conn_handle;

      /* Initiate MTU exchange after connection established */
      ble_gattc_exchange_mtu(conn_handle, NULL, NULL);

      /* Update connection parameters for higher throughput */
      struct ble_gap_upd_params params = {
          .itvl_min = 6,              /* 7.5ms (6 * 1.25ms) */
          .itvl_max = 12,             /* 15ms (12 * 1.25ms) */
          .latency = 0,               /* No slave latency */
          .supervision_timeout = 100, /* 1s (100 * 10ms) */
          .min_ce_len = 0,
          .max_ce_len = 0,
      };
      ble_gap_update_params(conn_handle, &params);
      break;

    case BLE_GAP_EVENT_DISCONNECT:
      //      MODLOG_DFLT(INFO, "disconnect; reason=%d\n",
      //      event->disconnect.reason);

      /* Connection terminated; resume advertising */
      blehr_advertise();
      break;

    case BLE_GAP_EVENT_ADV_COMPLETE:
      //      MODLOG_DFLT(INFO, "adv complete\n");
      blehr_advertise();
      break;

    case BLE_GAP_EVENT_SUBSCRIBE:
      //      MODLOG_DFLT(INFO,
      //                  "subscribe event; cur_notify=%d\n value handle; "
      //                  "val_handle=%d\n",
      //                  event->subscribe.cur_notify, hrs_hrm_handle);
      if (event->subscribe.attr_handle == hrs_hrm_handle) {
        notify_state = event->subscribe.cur_notify;
      } else if (event->subscribe.attr_handle != hrs_hrm_handle) {
        notify_state = event->subscribe.cur_notify;
      }
      //      ESP_LOGI("BLE_GAP_SUBSCRIBE_EVENT", "conn_handle from
      //      subscribe=%d",
      //               conn_handle);
      break;

    case BLE_GAP_EVENT_MTU:
      printf("MTU update event; conn_handle=%d mtu=%d\n",
             event->mtu.conn_handle, event->mtu.value);
      break;

    case BLE_GAP_EVENT_CONN_UPDATE:
      printf("Connection update event; status=%d\n", event->conn_update.status);
      break;
  }

  return 0;
}

/**
 * @brief Called when the BLE stack is synchronized
 *
 * Retrieves the device address and starts advertising.
 */
static void blehr_on_sync(void) {
  int rc;

  rc = ble_hs_id_infer_auto(0, &blehr_addr_type);
  assert(rc == 0);

  uint8_t addr_val[6] = {0};
  rc = ble_hs_id_copy_addr(blehr_addr_type, addr_val, NULL);

  //  MODLOG_DFLT(INFO, "Device Address: ");
  //  print_addr(addr_val);
  //  MODLOG_DFLT(INFO, "\n");

  /* Begin advertising */
  blehr_advertise();
}

/**
 * @brief Called when the BLE stack is reset
 *
 * @param reason Reason for the reset
 */
static void blehr_on_reset(int reason) {
  //  MODLOG_DFLT(ERROR, "Resetting state; reason=%d\n", reason);
}

/**
 * @brief BLE host task
 *
 * Main task for the NimBLE host, runs the NimBLE port.
 *
 * @param param Task parameters (unused)
 */
void blehr_host_task(void *param) {
  //  ESP_LOGI(tag, "BLE Host Task Started");
  /* This function will return only when nimble_port_stop() is executed */
  nimble_port_run();

  nimble_port_freertos_deinit();
}

/**
 * @brief Initializes the BLE subsystem
 *
 * Sets up the NimBLE stack, configures services, and starts advertising.
 */
void ble_init(void) {
  int rc;

  /* Initialize NVS — it is used to store PHY calibration data */
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ret = nimble_port_init();
  if (ret != ESP_OK) {
    //    MODLOG_DFLT(ERROR, "Failed to init nimble %d \n", ret);
    return;
  }

  /* Initialize the NimBLE host configuration */
  ble_hs_cfg.sync_cb = blehr_on_sync;
  ble_hs_cfg.reset_cb = blehr_on_reset;

  /* Set preferred MTU size */
  ble_att_set_preferred_mtu(512);

  /* Set preferred PHY to 2M */
  ble_gap_set_prefered_default_le_phy(BLE_GAP_LE_PHY_2M_MASK,
                                      BLE_GAP_LE_PHY_2M_MASK);

  rc = ble_blink_init();
  assert(rc == 0);

  /* Set the default device name */
  rc = ble_svc_gap_device_name_set(device_name);
  assert(rc == 0);

  /* Start the task */
  nimble_port_freertos_init(blehr_host_task);
}
