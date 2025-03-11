
#include <M5Unified.h>

#include "my_mrubydef.h"

static void class_m5_update(mrb_vm *vm, mrb_value *v, int argc) { M5.update(); }

static void  // see definition of "enum board_t" in M5GFX/src/lgfx/boards.hpp
class_m5_board(mrb_vm *vm, mrb_value *v, int argc) {
  SET_INT_RETURN(M5.getBoard());
}

#define VER_N_STR(n) VER_N_STR_H(n)
#define VER_N_STR_H(n) #n
static void class_m5_version(mrb_vm *vm, mrb_value *v,
                             int argc) {  // a little small challenge
  SET_RETURN(mrbc_string_new_cstr(
      vm, VER_N_STR(M5UNIFIED_VERSION_MAJOR) "." VER_N_STR(
              M5UNIFIED_VERSION_MINOR) "." VER_N_STR(M5UNIFIED_VERSION_PATCH)));

  //    char buf[32];
  //    snprintf(buf, sizeof(buf), "%d.%d.%d", M5UNIFIED_VERSION_MAJOR,
  //    M5UNIFIED_VERSION_MINOR, M5UNIFIED_VERSION_PATCH);
  //    SET_RETURN(mrbc_string_new_cstr(vm, buf));
}

mrbc_value failed_object;

void false_return(mrb_vm *vm, mrb_value *v, int argc) { SET_FALSE_RETURN(); }
void true_return(mrb_vm *vm, mrb_value *v, int argc) { SET_TRUE_RETURN(); }

void class_m5_init() {
  mrb_class *class_m5;
  class_m5 = mrbc_define_class(0, "M5", mrbc_class_object);
  mrbc_define_method(0, class_m5, "update", class_m5_update);
  mrbc_define_method(0, class_m5, "board", class_m5_board);
  mrbc_define_method(0, class_m5, "unified_version", class_m5_version);

  mrb_class *class_failed =
      mrbc_define_class(0, "FailObject", mrbc_class_object);
  mrbc_define_method(0, class_failed, "method_missing", false_return);
  failed_object = mrbc_instance_new(0, class_failed, 0);
}
