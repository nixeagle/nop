#include "kpanic.h"
#include "kernel/string/string.h"
#include "kernel/text_mode/text_mode.h"

namespace kernel {
  namespace panic {
    using kernel::text_mode::puts;
    using kernel::text_mode::put_hex;
    void header (const char* message, const char* function,
                 int line, const char* file, const char* timestamp) {
      kernel::text_mode::clear_screen();
      puts("          ===-- NOP PANIC --===", 0, 0);
      puts("::   ", 1, 0);
      puts(message, 1, 6);
      puts("Function:", 2, 0);
      puts(function, 2, 10);
      puts("File: ", 3, 15);
      puts(file, 3, 21);
      puts("Line: ", 3, 0);
      put_hex(line, 3, 6);
      puts("Timestamp: ", 4, 15);
      puts(timestamp, 4, 27);
      return;
    }

    void kpanic (const char* issue ,const char* message, const char* function,
                 int line, const char* file, const char* timestamp) {
      header(message, function, line, file, timestamp);
      asm("hlt");
    }
  }
}
