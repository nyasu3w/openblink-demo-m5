#include "c_speaker.h"

#include <M5Unified.h>

#include "my_mrubydef.h"

#ifdef USE_SPEAKER

static void c_speaker_tone(mrb_vm *vm, mrb_value *v, int argc) {
  if (argc < 2) {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "wrong number of arguments");
    SET_FALSE_RETURN();
    return;
  }

  int freq = MRBC_ARG_F(1);
  int duration = MRBC_ARG_I(2);
  int channel = (argc > 2) ? MRBC_ARG_I(3) : -1;
  bool stop_current = (argc < 4 || (GET_ARG(4).tt != MRBC_TT_NIL &&
                                    GET_ARG(4).tt != MRBC_TT_FALSE));
  M5.Speaker.tone(freq, duration, channel, stop_current);
}

static void c_speaker_stop(mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 0) {
    int channel = MRBC_ARG_I(1);
    M5.Speaker.stop(channel);
  } else {
    M5.Speaker.stop();
  }
}

static void c_speaker_set_volume(mrb_vm *vm, mrb_value *v, int argc) {
  if (argc < 1) {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "wrong number of arguments");
    SET_FALSE_RETURN();
    return;
  }

  int volume = MRBC_ARG_I(1);
  M5.Speaker.setVolume(volume);
}

static void c_speaker_get_volume(mrb_vm *vm, mrb_value *v, int argc) {
  int volume = M5.Speaker.getVolume();
  SET_INT_RETURN(volume);
}

static void c_speaker_is_playing(mrb_vm *vm, mrb_value *v, int argc) {
  int channel = (argc > 0) ? MRBC_ARG_I(1) : -1;
  if (M5.Speaker.isPlaying(channel)) {
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void class_speaker_init() {
  mrbc_class *speaker = mrbc_define_class(0, "Speaker", mrbc_class_object);
  mrbc_define_method(0, speaker, "tone", c_speaker_tone);
  mrbc_define_method(0, speaker, "stop", c_speaker_stop);
  mrbc_define_method(0, speaker, "set_volume", c_speaker_set_volume);
  mrbc_define_method(0, speaker, "volume=", c_speaker_set_volume);
  mrbc_define_method(0, speaker, "get_volume", c_speaker_get_volume);
  mrbc_define_method(0, speaker, "volume", c_speaker_set_volume);
  mrbc_define_method(0, speaker, "is_playing?", c_speaker_is_playing);
}

#endif  // USE_SPEAKER
