#include "gdt.h"

namespace kernel {
  namespace gdt {
    int GdtEntry::setLimit(size_t limit) {
      if (1048576 < limit) {
        return 1; /// \retval 1 \a limit is larger then 2^20.
      } else {
        this->limit = limit & 0xFFFF;
        flags_and_limit |= static_cast<uint8_t>(limit >> 16);
        return 0; /// \suc0
      }
    }

    int GdtEntry::setBase(size_t base) {
      this->base = base & 0xFFFF;
      base2 = (base >> 16) & 0xFF;
      base3 = static_cast<uint8_t>(base >> 24) & 0xFF;
      return 0;
    }

    int GdtEntry::setFlags(uint8_t flags) {
      if (0 != (flags & 0b00110000)) {
        return 1; /// \retval 1 \a flags 4th and 5th bit are not 0.
      } else {
        flags_and_limit = static_cast<uint8_t>(flags_and_limit | (flags & 0b11000000));
        return 0;
      }
    }
  }
}
