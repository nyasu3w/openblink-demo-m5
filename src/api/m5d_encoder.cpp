
#include "mrubyc.h"
#include "../lib/esp32_encoder/ESP32Encoder.h"
#include "m5d_encoder.h"

#define DIAL_ENCODER_PIN_A 41
#define DIAL_ENCODER_PIN_B 40

ESP32Encoder encoder;

static void c_encoder_getcount(mrb_vm *vm, mrb_value *v, int argc) {
    SET_INT_RETURN(encoder.getCount());
}  


void api_encoder_define(void){
    encoder.attachHalfQuad(DIAL_ENCODER_PIN_A, DIAL_ENCODER_PIN_B);
    encoder.clearCount();

    mrbc_define_class(0, "Encoder", mrbc_class_object);
    mrbc_define_method(0, mrbc_class_object, "get_count", c_encoder_getcount);
}