#include "mrubyc.h"

extern "C" {
    extern mrbc_value failed_object;
    void false_return(mrb_vm *vm, mrb_value *v, int argc);
    void true_return(mrb_vm *vm, mrb_value *v, int argc);
    void class_m5_init();
    void my_mrubyc_init();
}
