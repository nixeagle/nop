#include "keyboard.h"
#include "kernel/text_mode/text_mode.h"
#include "kernel/asm/out.h"
#include "kernel/asm/in.h"
#include "keysyms.h"
namespace kernel {
  namespace drivers {
    namespace keyboard {
      using kernel::inlasm::outb;
      using kernel::inlasm::inb;
      using kernel::text_mode::puts;
      using kernel::text_mode::put_hex;
      using kernel::text_mode::put_char;
      static uint32_t keypress_number = 0;
      void handleEvent() {
        uint8_t scan_code = inb(0x60);

        static bool shift = false; // state of the shift key.
        static bool control = false; // state of the control key.

        switch(scan_code) {
        case 0x2a:
          shift = true;
          break;
        case 0xaa:
          shift = false;
          break;
        default:
          puts("Last 8 scancodes", 11, 0);
          put_hex(scan_code, 12, (keypress_number++ % 8) * 10);
          put_char(scan_code + 67, 10, 30);

          put_hex(KeySyms::kF12, 15, 0);

          break;
        }
      }
    }
  }
}
