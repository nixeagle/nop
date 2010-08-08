#pragma once
#include "kernel/gdt/gdt.h"
#include "kernel/gdt/descriptor.h"

namespace kernel {
  /// Inline assembler
  namespace inlasm {
    __attribute__ ((gnu_inline)) inline void
    lgdt(kernel::gdt::BaseDescriptor<kernel::gdt::GdtEntry>* (descriptor)) {
      asm ("lgdt %0;" : : "m" (descriptor));
    }
  }
}
