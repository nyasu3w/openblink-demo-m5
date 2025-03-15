#include "c_m5u.h"

#include <M5Unified.h>

#include "c_canvas.h"
#include "c_display_button.h"
#include "c_m5.h"
#include "c_speaker.h"
#include "c_touch.h"
#include "c_utils.h"
#include "mrubyc.h"

void init_c_m5u() {
  class_m5_init();
  class_display_button_init();
  class_utils_init();
  class_touch_init();
  class_canvas_init();
  class_speaker_init();
}