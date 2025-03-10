#include "c_utils.h"

#include <M5Unified.h>

#include "my_mrubydef.h"

#ifdef USE_TEMPORAL_RANDOM_FUNCTION
static void class_utils_randomseed(mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 0) {
    randomSeed(val_to_i(vm, v, GET_ARG(1), argc));
  } else {
    randomSeed(analogRead(0));
  }
  SET_TRUE_RETURN();
}

static void class_utils_random(mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 1) {
    SET_INT_RETURN(random(val_to_i(vm, v, GET_ARG(1), argc),
                          val_to_i(vm, v, GET_ARG(2), argc)));
  } else if (argc > 0) {
    SET_INT_RETURN(random(val_to_i(vm, v, GET_ARG(1), argc)));
  } else {
    SET_INT_RETURN(random());
  }
}
#endif  // USE_TEMPORAL_RANDOM_FUNCTION

static void class_utils_millis(mrb_vm *vm, mrb_value *v, int argc) {
  SET_INT_RETURN(xTaskGetTickCount());  // 後で直す。適当にも程がある
}
static void class_utils_delay(mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 0)
    vTaskDelay(
        val_to_i(vm, v, GET_ARG(1), argc));  // 後で直す。適当にも程がある
}

void class_utils_init() {
  mrbc_class *class_utils = mrbc_define_class(0, "Utils", mrbc_class_object);
  mrbc_define_method(0, class_utils, "millis", class_utils_millis);
  mrbc_define_method(0, class_utils, "delay", class_utils_delay);

#ifdef USE_TEMPORAL_RANDOM_FUNCTION
  mrbc_define_method(0, class_utils, "randomseed", class_utils_randomseed);
  mrbc_define_method(0, class_utils, "random", class_utils_random);
#endif  // USE_TEMPORAL_RANDOM_FUNCTION
}
