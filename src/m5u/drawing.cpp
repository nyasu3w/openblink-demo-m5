//
// Common drawing functions for M5GFX, M5Canvas
//

#include "drawing.h"

#include <M5Unified.h>

#include "my_mrubydef.h"

void draw_set_text_size(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 0) {
    int sz = MRBC_ARG_I(1);
    dst->setTextSize(sz);
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void draw_print(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  int r = 0;
  for (int i = 1; i <= argc; i++) {
    const char *str = MRBC_ARG_S(i);
    r += dst->print(str);
  }
  SET_INT_RETURN(r);
}

void draw_puts(LovyanGFX *dst, mrb_vm *vm, mrb_value v[], int argc) {
  int r = 0;
  if (argc == 0) {
    r += dst->println();
  }
  for (int i = 1; i <= argc; i++) {
    const char *str = MRBC_ARG_S(i);
    r += dst->println(str);
  }
  SET_INT_RETURN(r);
}

void draw_clear(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  int color = 0;
  if (argc > 0) {
    color = MRBC_ARG_I(1);
  }

  dst->clearDisplay(color);
  dst->setCursor(0, 0);
  SET_TRUE_RETURN();
}

void draw_set_text_color(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 1) {
    dst->setTextColor((int8_t)MRBC_ARG_I(1), (int8_t)MRBC_ARG_I(2));
    SET_TRUE_RETURN();
  } else if (argc > 0) {
    dst->setTextColor((int16_t)MRBC_ARG_I(1));
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void draw_set_cursor(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 1) {
    dst->setCursor(MRBC_ARG_I(1), MRBC_ARG_I(2));
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void draw_get_cursor(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  mrbc_value ret = mrbc_array_new(vm, 2);
  mrbc_value x = mrbc_fixnum_value(dst->getCursorX());
  mrbc_value y = mrbc_fixnum_value(dst->getCursorY());
  mrbc_array_set(&ret, 0, &x);
  mrbc_array_set(&ret, 1, &y);
  SET_RETURN(ret);
}

void draw_get_dimension(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  mrbc_value ret = mrbc_array_new(vm, 2);
  mrbc_value w = mrbc_fixnum_value(dst->width());
  mrbc_value h = mrbc_fixnum_value(dst->height());
  mrbc_array_set(&ret, 0, &w);
  mrbc_array_set(&ret, 1, &h);
  SET_RETURN(ret);
}

//
// for USE_DISPLAY_GRAPHICS
//

#ifdef USE_DISPLAY_GRAPHICS
void draw_fill_rect(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 4) {
    int x = MRBC_ARG_I(1);
    int y = MRBC_ARG_I(2);
    int w = MRBC_ARG_I(3);
    int h = MRBC_ARG_I(4);
    int color = MRBC_ARG_I(5);
    dst->fillRect(x, y, w, h, color);
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void draw_draw_rect(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 4) {
    int x = MRBC_ARG_I(1);
    int y = MRBC_ARG_I(2);
    int w = MRBC_ARG_I(3);
    int h = MRBC_ARG_I(4);
    int color = MRBC_ARG_I(5);
    dst->drawRect(x, y, w, h, color);
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void draw_draw_line(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 4) {
    int x1 = MRBC_ARG_I(1);
    int y1 = MRBC_ARG_I(2);
    int x2 = MRBC_ARG_I(3);
    int y2 = MRBC_ARG_I(4);
    int color = MRBC_ARG_I(5);
    dst->drawLine(x1, y1, x2, y2, color);
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void draw_flll_circle(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 3) {
    int x = MRBC_ARG_I(1);
    int y = MRBC_ARG_I(2);
    int r = MRBC_ARG_I(3);
    int color = MRBC_ARG_I(4);
    dst->fillCircle(x, y, r, color);
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void draw_draw_circle(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 3) {
    int x = MRBC_ARG_I(1);
    int y = MRBC_ARG_I(2);
    int r = MRBC_ARG_I(3);
    int color = MRBC_ARG_I(4);
    dst->drawCircle(x, y, r, color);
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

#ifdef USE_FILE_FUNCTION

static void draw_draw_pic_file(LovyanGFX *dst, draw_pic_type t, mrb_vm *vm,
                               mrb_value *v, int argc) {
  if (argc < 3) {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "too few arguments");
    return;
  }
  mrbc_value file = MRBC_ARG(1);
  int r = mrbc_obj_is_kind_of(&file, class_file);
  if (r == 0) {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "not a file");
    SET_FALSE_RETURN();
    return;
  }
  File *f = *(File **)file.instance->data;
  int x = MRBC_ARG_I(2);
  int y = MRBC_ARG_I(3);

  draw_draw_pic_stream(dst, t, f, x, y);

  SET_TRUE_RETURN();
}

void draw_draw_bmp(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  draw_draw_pic_file(dst, bmp, vm, v, argc);
}

void draw_draw_jpg(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  draw_draw_pic_file(dst, jpg, vm, v, argc);
}

void draw_draw_png(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  draw_draw_pic_file(dst, png, vm, v, argc);
}

#endif  // USE_FILE_FUNCTION

static void draw_draw_pic_mem(LovyanGFX *dst, draw_pic_type t,
                              const uint8_t *mem, size_t memsize, int x,
                              int y) {
  switch (t) {
    case bmp:
      dst->drawBmp(mem, memsize, x, y);
      break;
    case jpg:
      dst->drawJpg(mem, memsize, x, y);
      break;
    case png:
      dst->drawPng(mem, memsize, x, y);
      break;
  }
}
void draw_draw_pic_str(LovyanGFX *dst, draw_pic_type t, mrb_vm *vm,
                       mrb_value *v, int argc) {
  if (argc < 3) {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "too few arguments");
    return;
  }
  if (GET_ARG(1).tt != MRBC_TT_STRING) {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "not a string");
    SET_FALSE_RETURN();
    return;
  }
  const uint8_t *mem = GET_ARG(1).string->data;
  size_t memsize = GET_ARG(1).string->size;

  int x = MRBC_ARG_I(2);
  int y = MRBC_ARG_I(3);

  draw_draw_pic_mem(dst, t, mem, memsize, x, y);

  SET_TRUE_RETURN();
}

void draw_draw_bmpstr(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  draw_draw_pic_str(dst, bmp, vm, v, argc);
}

void draw_draw_jpgstr(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  draw_draw_pic_str(dst, jpg, vm, v, argc);
}

void draw_draw_pngstr(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  draw_draw_pic_str(dst, png, vm, v, argc);
}

void draw_set_rotation(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc > 0) {
    int rotation = MRBC_ARG_I(1);
    dst->setRotation(rotation);
    SET_TRUE_RETURN();
  } else {
    SET_FALSE_RETURN();
  }
}

void draw_scroll(LovyanGFX *dst, mrb_vm *vm, mrb_value *v, int argc) {
  if (argc == 2) {
    int dx = MRBC_ARG_I(1);
    int dy = MRBC_ARG_I(2);
    dst->scroll(dx, dy);
  } else {
    mrbc_raise(vm, MRBC_CLASS(ArgumentError), "dx and dy");
    SET_FALSE_RETURN();
  }
}

#endif  // USE_DISPLAY_GRAPHICS
