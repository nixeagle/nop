#include "gdt.h"
#include "kernel/text_mode/text_mode.h"

namespace kernel {
  namespace gdt {
    using kernel::text_mode::puts;
    using kernel::text_mode::put_hex;
    int GdtEntry::inspect(uint8_t line) {

      put_hex(limit, line, 3);
      puts("limit: ", line, 0);
      put_hex(flags_and_limit, line, 19);
      puts("flags/limit: ", line, 12);
      put_hex(access_byte, line, 30);
      puts("access: ", line, 28);
      put_hex(base, line, 42);
      puts("Base1: ", line, 39);
      put_hex(base2, line, 52);
      puts("Base 2: ", line, 51);
      put_hex(base3, line, 62);
      puts("Base 3: ", line, 61);
      return 0;
    }

    int GdtDescriptor::inspect(void) {
      puts("------ GDT descriptor data ------", 1, 0);
      puts("Base:", 2, 0);
      put_hex(reinterpret_cast<unsigned int>(getBase()), 2, 6);
      puts("Limit:", 2, 15);
      put_hex(getLimit(), 2, 22);
      puts("Entry Count:", 2, 32);
      put_hex(getEntryCount(), 2, 45);

      for(int i = 0; i < getEntryCount(); i++) {
        getBase()[i].inspect(static_cast<uint8_t>(i+3));
      }
      return 0;
    }
  }
}
