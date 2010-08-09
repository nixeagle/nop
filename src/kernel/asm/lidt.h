#pragma once
#include "kernel/idt/idt.h"
#include "kernel/gdt/descriptor.h"

namespace kernel {
  namespace inlasm {
    __attribute__((gnu_inline))
    inline void lidt (kernel::gdt::BaseDescriptor<kernel::idt::IdtEntry>* descriptor) {
      asm("lidt %0" : : "m" (descriptor));
    }
  }
}
