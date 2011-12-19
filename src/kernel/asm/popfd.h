#pragma once
#include "types.h"

namespace kernel {
  namespace as {
    /// Set the flags register from the stack, popping the value.
    /// For C++ compatability we also push the value onto the stack before
    /// executing this assembly instruction. The resulting machine state of
    /// the stack is unmodified after this instruction.
    inline void popfd(u32 flags) {
      asm volatile("push %0;popf": "=r"(flags));
      return;
    }
  }
}
