#include "keyboard.h"
#include "kernel/text_mode/text_mode.h"
#include "kernel/asm/out.h"
#include "kernel/asm/in.h"
#include "keysyms.h"
#include "kernel/virtual-console/virtual-console.h"
#include "global.h"
#include "key-event.h"
#include "kernel/memory/memory.h"

namespace kernel {
  namespace drivers {
    namespace keyboard {
      using kernel::inlasm::outb;
      using kernel::inlasm::inb;
      using kernel::text_mode::puts;
      using kernel::text_mode::put_hex;
      using kernel::text_mode::put_char;
      using kernel::VirtualConsole;
      static uint32_t keypress_number = 0;
      void handleEvent() {
        uint8_t scan_code = inb(0x60);

        // static bool shift = false; // state of the shift key.
        // static bool control = false; // state of the control key.

        // switch(scan_code) {
        // case 0x36: // right shift
        // case 0x2a: // left shift
        //   shift = true;
        //   break;
        // case 0xb6: // Right shift break
        // case 0xaa: // left shift break
        //   shift = false;
        //   break;
        // default:
        //   //          puts("Last 8 scancodes", 11, 0);
        //   //          put_hex(scan_code, 12, (keypress_number++ % 8) * 10);
        //   //          KeyEvent* event;
        //   //          if(shift) {
        //     //event = new KeyEvent(scan_code, shift, control);
        //     //            put_char(keysym_us_shift[scan_code], 10, 30);
        //   //          } else {
        //     //event = new KeyEvent(scan_code, shift, control);
        //     // put_char(keysym_us[scan_code], 10, 30);
        //   //          }
        //   // KeyEvent* a = new KeyEvent(scan_code, shift, control);

        //   break;
        // }
        KeyEvent* event = new KeyEvent(scan_code);
        (kernel::global::key_event)(VirtualConsole::currentConsole(), event);
        // delete event;
      }
    }
  }
}
