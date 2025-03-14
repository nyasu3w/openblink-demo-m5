#include <M5Unified.h>
#include "my_mrubydef.h"
#include "c_touch.h"

#ifdef USE_TOUCH

static void
class_touch_get_count(mrb_vm *vm, mrb_value *v, int argc)
{
    SET_INT_RETURN(M5.Touch.getCount());
}

static void
class_touch_get_detail(mrb_vm *vm, mrb_value *v, int argc)
{
    int no = (argc>0)? GET_INT_ARG(1) : 0;
    if(no<M5.Touch.getCount()){
        auto detail = M5.Touch.getDetail(no);
        mrb_value ret = mrbc_array_new(vm, 5);
        mrb_value a[5];
        a[0] = mrbc_integer_value(detail.x);
        a[1] = mrbc_integer_value(detail.y);
        a[2] = mrbc_integer_value(detail.prev_x);
        a[3] = mrbc_integer_value(detail.prev_y);
        a[4] = mrbc_integer_value(detail.id); 
        mrbc_array_set(&ret, 0, &a[0]);
        mrbc_array_set(&ret, 1, &a[1]);
        mrbc_array_set(&ret, 2, &a[2]);
        mrbc_array_set(&ret, 3, &a[3]);
        mrbc_array_set(&ret, 4, &a[4]);
        SET_RETURN(ret);
        return;
    }
    SET_NIL_RETURN();
}

static void class_touch_wasclicked(mrb_vm *vm, mrb_value *v, int argc)
{
    int no = (argc>0)? val_to_i(vm,v,GET_ARG(1),argc) : 0;
    if(no<M5.Touch.getCount()){
        if(M5.Touch.getDetail(no).wasClicked()){  
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
}

static void class_touch_ispressed(mrb_vm *vm, mrb_value *v, int argc)
{
    int no = (argc>0)? val_to_i(vm,v,GET_ARG(1),argc)  : 0;
    if(no<M5.Touch.getCount()){
        if(M5.Touch.getDetail(no).isPressed()){
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
}

static void class_touch_isreleased(mrb_vm *vm, mrb_value *v, int argc)
{
    int no = (argc>0)?  val_to_i(vm,v,GET_ARG(1),argc)  : 0;
    if(no<M5.Touch.getCount()){
        if(M5.Touch.getDetail(no).isReleased()){
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
}

static void class_touch_isholding(mrb_vm *vm, mrb_value *v, int argc)
{
    int no = (argc>0)?  val_to_i(vm,v,GET_ARG(1),argc)  : 0;
    if(no<M5.Touch.getCount()){
        if(M5.Touch.getDetail(no).isHolding()){
            SET_TRUE_RETURN();
            return;
        }
    }
    SET_FALSE_RETURN();
}


void class_touch_init(){
    if(M5.Touch.isEnabled()){
        mrb_class *class_touch;
        class_touch = mrbc_define_class(0, "Touch", mrbc_class_object);
        mrbc_define_method(0, class_touch, "available?", true_return);
        mrbc_define_method(0, class_touch, "count", class_touch_get_count);
        mrbc_define_method(0, class_touch, "detail", class_touch_get_detail);
        mrbc_define_method(0, class_touch, "was_clicked?", class_touch_wasclicked);
        mrbc_define_method(0, class_touch, "is_pressed?", class_touch_ispressed);
        mrbc_define_method(0, class_touch, "is_released?", class_touch_isreleased);
        mrbc_define_method(0, class_touch, "is_holding?", class_touch_isholding);
    } else {
        mrbc_set_const(mrbc_str_to_symid("Touch"), &failed_object);
    }
}

#endif // USE_TOUCH