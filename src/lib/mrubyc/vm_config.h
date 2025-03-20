/*! @file
  @brief
  Global configuration of mruby/c VM's

  <pre>
  Copyright (C) 2015- Kyushu Institute of Technology.
  Copyright (C) 2015- Shimane IT Open-Innovation Center.

  This file is distributed under BSD 3-Clause License.
  </pre>
*/

void on_mrbc_exception(const void *ptr);
#define MRBC_ABORT_BY_EXCEPTION(vm) on_mrbc_exception( &vm->exception )

