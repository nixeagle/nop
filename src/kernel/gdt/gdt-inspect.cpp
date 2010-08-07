#include "gdt.h"
#include "kernel/text_mode/text_mode.h"

namespace kernel {
  namespace gdt {
    int GdtEntry::inspect(uint8_t line) {
      using kernel::text_mode::puts;
      using kernel::text_mode::put_hex;


      put_hex(limit, line, 3);
      puts("limit: ", line, 0);
      put_hex(flags_and_limit, line, 19);
      puts("flags/limit: ", line, 12);
      put_hex(access_byte, line, 30);
      puts("access: ", line, 28);
      put_hex(base, line, 43);
      puts("Base 1: ", line, 39);
      put_hex(base2, line, 54);
      puts("Base 2: ", line, 52);
      put_hex(base3, line, 65);
      puts("Base 3: ", line, 63);
      return 0;
    }
  }
}
