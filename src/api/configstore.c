#include "mrubyc.h"
#include "configstore.h"

#include "../app/configstore.h"

static void c_configstore_get_int(mrb_vm *vm, mrb_value *v, int argc) {
    const char* key = (const char*)GET_STRING_ARG(1);
    int r = config_data_get_int(key);
    SET_INT_RETURN(r);
}

static void c_configstore_get_string(mrb_vm *vm, mrb_value *v, int argc) {
    const char* key = (const char*)GET_STRING_ARG(1);
    struct config_item_data* data = config_data_get(key);
    if(data == NULL) {
        SET_NIL_RETURN();
    }
    SET_RETURN( mrbc_string_new(vm, data->data, data->size) );  
} 

static void c_configstore_set_int(mrb_vm *vm, mrb_value *v, int argc) {
    const char* key = (const char*)GET_STRING_ARG(1);
    int value = GET_INT_ARG(2);
    if (config_data_set_int(key, value)) {
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void c_configstore_set_string(mrb_vm *vm, mrb_value *v, int argc) {
    const char* key = (const char*)GET_STRING_ARG(1);
    mrbc_value arg = GET_ARG(2);
    if (arg.tt != MRBC_TT_STRING) {
        SET_FALSE_RETURN();
    }
    if (config_data_set_string(key, arg.string->data, arg.string->size)) {
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void c_configstore_save(mrb_vm *vm, mrb_value *v, int argc) {
    if (config_data_save()) {
        SET_TRUE_RETURN();
    } else {
        SET_FALSE_RETURN();
    }
}

static void c_configstore_debug(mrb_vm *vm, mrb_value *v, int argc) {
    config_data_debug();
    SET_TRUE_RETURN();
}

void api_configstore_define(void){


    // Define the ConfigStore class in mruby/c
    mrb_class *class_configstore = mrbc_define_class(0, "ConfigStore", mrbc_class_object);    
    mrbc_define_method(0, class_configstore, "get_int", c_configstore_get_int);
    mrbc_define_method(0, class_configstore, "get_string", c_configstore_get_string);
    mrbc_define_method(0, class_configstore, "set_int", c_configstore_set_int);
    mrbc_define_method(0, class_configstore, "set_string", c_configstore_set_string);
    mrbc_define_method(0, class_configstore, "save", c_configstore_save);
    mrbc_define_method(0, class_configstore, "debug", c_configstore_debug);
}
