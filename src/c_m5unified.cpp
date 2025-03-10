
#include "c_m5unified.h"
#include <M5Unified.h>
#include "mrubyc.h"

static void c_display_puts(mrb_vm *vm, mrb_value *v, int argc) {
    int r=0;
    if(argc==0) {
        r+=M5.Display.println();
    }
    for(int i=1; i<=argc; i++){
        r+=M5.Display.println((const char*) GET_STRING_ARG(i));
    }
    SET_INT_RETURN(r);
}

static void c_display_settextcolor(mrb_vm *vm, mrb_value *v, int argc) {
    if(argc==0) {
        SET_FALSE_RETURN();
    } else if(argc==1){
        M5.Display.setTextColor((uint16_t)GET_INT_ARG(1));
    } else if(argc==2){
        M5.Display.setTextColor((uint16_t)GET_INT_ARG(1),(uint16_t)GET_INT_ARG(2));
    }
}

void init_c_m5unified() {
  static bool need_init = true;
  if(need_init){
    auto cfg = M5.config();
    M5.begin(cfg);
    printf("INITIALIZED M5Unified\n");
  }

  printf("INITIALIZING M5UNIFIED Lib\n");
  M5.Display.setTextSize(2);
  mrb_class *display_class;
  display_class = mrbc_define_class(0, "Display", mrbc_class_object);
  mrbc_define_method(0, display_class, "puts", c_display_puts);
  mrbc_define_method(0, display_class, "set_text_color", c_display_settextcolor);

}
