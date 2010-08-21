#include "key-event.h"
#include "kernel/drivers/keyboard/keysyms.h"

namespace kernel {
  namespace drivers {
    namespace keyboard {
      ModifierKeys KeyEvent::modifier = {false,false,false,false,false,false,false,false};
      char KeyEvent::getAsciiKey(void) const {
        if (modifier.shift) {
          return static_cast<char>(keysym_us_shift[code]);
        } else {
          return static_cast<char>(keysym_us[code]);
        }
      }

      bool KeyEvent::handleModifierKey(void) {
        switch(code) {
        case 0x36: // right shift
        case 0x2a: // left shift
          modifier.shift = true;
          return true;
        case 0xb6: // Right shift break
        case 0xaa: // Left shift break
          modifier.shift = false;
          return true;
        default:
          return false;
        }
      }

      KeyEvent::KeyEvent(uint32_t code) : code(code) {
        handleModifierKey();
      }
    }
  }
}
