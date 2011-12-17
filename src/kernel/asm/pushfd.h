#pragma once
#include "types.h"

namespace kernel {
  namespace as {
    /// Push the flags register on the stack.
    /// For C++ compatability we also pop this pushed value off the stack
    /// and return the value. 
    /// @note that we do not care about performance with this instruction
    /// at all as eflags accesses are not done in performance critical
    /// portions of the OS. 
    inline u32 pushfd(void) {
      u32 eflags = 0xffffffff;
      asm ("pushfd; pop eax": "=eax"(eflags));
      return eflags;
    }
  }
}
