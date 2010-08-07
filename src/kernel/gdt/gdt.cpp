#include "gdt.h"

namespace kernel {
  namespace gdt {
    int GdtEntry::setLimit(size_t limit) {
      if (1048576 < limit) {
        return 1; /// \retval 1 \a limit is larger then 2^20.
      } else {
        this->limit = limit & 0xFFFF;
        flags_and_limit = flags_and_limit | static_cast<uint8_t>(limit >> 16);
        return 0;
      }
    }
  }
}
