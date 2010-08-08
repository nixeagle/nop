#pragma once
#include "kernel/gdt/gdt.h"

namespace kernel {
  /// Inline assembler
  namespace inlasm {
    __attribute__ ((gnu_inline)) inline void
    lgdt(kernel::gdt::GdtDescriptor* descriptor) {
      asm ("lgdtl %0;" : : "m" (descriptor));
    }
  }
}
