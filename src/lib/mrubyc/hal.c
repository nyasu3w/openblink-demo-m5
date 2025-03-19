/*! @file
  @brief
  Hardware abstraction layer
        for ESP32

  <pre>
  Copyright (C) 2015- Kyushu Institute of Technology.
  Copyright (C) 2015- Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.
  </pre>
*/

/***** Feature test switches ************************************************/
/***** System headers *******************************************************/
#include <stdio.h>
#include <string.h>

//#include "driver/periph_ctrl.h"
#include "driver/gptimer.h"

#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "soc/timer_group_struct.h"

/***** Local headers ********************************************************/
#include "hal.h"

/***** Constat values *******************************************************/
#define TIMER_DIVIDER 80

/***** Macros ***************************************************************/
/***** Typedefs *************************************************************/
/***** Function prototypes **************************************************/
/***** Local variables ******************************************************/
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

/***** Global variables *****************************************************/
/***** Signal catching functions ********************************************/
/***** Local functions ******************************************************/
#ifndef MRBC_NO_TIMER
//================================================================
/*!@brief
  Timer ISR function

*/

#endif

/***** Global functions *****************************************************/
#ifndef MRBC_NO_TIMER

//================================================================
/*!@brief
  initialize

*/

static bool IRAM_ATTR on_timer(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
  // ここにタイマー割り込み時の処理を記述
  mrbc_tick();
  return true; // タイマーを継続する場合
}

// タイマーの初期化関数
void hal_init(void) {
  // GPTimerの設定
  gptimer_config_t timer_config = {
      .clk_src = GPTIMER_CLK_SRC_APB,     // APBクロックソースを使用
      .direction = GPTIMER_COUNT_UP,      // カウントアップ
      .resolution_hz = 80000000 / TIMER_DIVIDER, // 分周後の分解能（例: 80MHz / TIMER_DIVIDER）
  };

  // タイマーハンドルの作成
  gptimer_handle_t timer = NULL;
  esp_err_t ret = gptimer_new_timer(&timer_config, &timer);
  if (ret != ESP_OK) {
      printf("タイマーの作成に失敗しました\n");
      return;
  }

  // アラーム設定
  gptimer_alarm_config_t alarm_config = {
      .alarm_count = MRBC_TICK_UNIT * 1000, // アラーム値（カウント数）
      .reload_count = 0,                    // 0からリロード
      .flags.auto_reload_on_alarm = true,   // アラーム時に自動リロード
  };
  gptimer_set_alarm_action(timer, &alarm_config);

  // コールバック関数の登録
  gptimer_event_callbacks_t cbs = {
      .on_alarm = on_timer, // アラーム時のコールバック
  };
  gptimer_register_event_callbacks(timer, &cbs, NULL);

  // タイマーを有効化
  gptimer_enable(timer);

  // カウンタを0に設定
  gptimer_set_raw_count(timer, 0);

  // タイマーを開始
  gptimer_start(timer);
}

//================================================================
/*!@brief
  enable interrupt

*/
void hal_enable_irq(void) { portEXIT_CRITICAL(&mux); }

//================================================================
/*!@brief
  disable interrupt

*/
void hal_disable_irq(void) { portENTER_CRITICAL(&mux); }

#endif /* ifndef MRBC_NO_TIMER */

//================================================================
/*!@brief
  abort program

  @param s	additional message.
*/
void hal_abort(const char *s) {
  if (s) {
    write(1, s, strlen(s));
  }

  abort();
}
