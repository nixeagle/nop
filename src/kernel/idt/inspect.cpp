#include "idt.h"

namespace kernel {
  namespace idt {
    using kernel::text_mode::puts;
    using kernel::text_mode::put_hex;
    void IdtEntry::inspect(size_t line_number) {
      put_hex(base_high, line_number, 0);
      put_hex(base_low, line_number, 10);
      put_hex(line_number, line_number, 40);
    }
  }
}
