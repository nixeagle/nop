#pragma once
#include "types.h"
namespace kernel {
  namespace inlasm {
    /// Output a \a byte to \a port.
    static inline void outb (uint16_t port, uint8_t value) {
      asm volatile("outb %0,%1"::"a"(value), "Nd" (port));
    }

    /// Output a \a word to \a port.
    static inline void outw (uint16_t port, uint16_t value) {
      asm volatile("outw %0,%1"::"a"(value), "Nd" (port));
    }
    /// Output a \a word to \a port.
    static inline void outl (uint16_t port, uint32_t value) {
      asm volatile("outl %0,%1"::"a"(value), "Nd" (port));
    }

  }
}
