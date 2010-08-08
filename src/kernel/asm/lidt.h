#pragma once
#include "kernel/idt/idt.h"

namespace kernel {
  namespace inlasm {
    __attribute__((gnu_inline))
    inline void lidt (kernel::idt::IdtDescriptor* descriptor) {
      asm("lidt %0" : : "m" (descriptor));
    }
  }
}
