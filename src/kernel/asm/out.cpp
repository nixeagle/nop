#include "out.h"

namespace kernel {
  namespace inlasm {
    void outb (uint8_t port, uint8_t value) {
      asm volatile("outb %0,%1"::"a"(value), "Nd" (port));
    }

    void outw (uint8_t port, uint16_t value) {
      asm volatile("outw %0,%1"::"a"(value), "Nd" (port));
    }

    void outl (uint8_t port, uint32_t value) {
      asm volatile("outl %0,%1"::"a"(value), "Nd" (port));
    }
  }
}
