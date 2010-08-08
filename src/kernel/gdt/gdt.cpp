#include "gdt.h"
#include "kernel/panic/kpanic.h"

namespace kernel {
  namespace gdt {
    void GdtEntry::setLimit(size_t limit) {
      if (1048576 < limit) {
        KPANIC(this, "Limit is larger then 2^20");
      } else {
        this->limit = limit & 0xFFFF;
        flags_and_limit |= static_cast<uint8_t>(limit >> 16);
      }
    }

    void GdtEntry::setBase(size_t base) {
      this->base = base & 0xFFFF;
      base2 = (base >> 16) & 0xFF;
      base3 = static_cast<uint8_t>(base >> 24) & 0xFF;
    }

    void GdtEntry::setFlags(uint8_t flags) {
      if (0 != (flags & 0b00110000)) {
        KPANIC(this, "4th and 5th bit in flags are not 0.");
      } else {
        flags_and_limit = static_cast<uint8_t>(flags_and_limit | (flags & 0b11000000));
      }
    }
  }
}
