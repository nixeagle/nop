#include "key-event.h"
#include "kernel/drivers/keyboard/keysyms.h"

namespace kernel {
  namespace drivers {
    namespace keyboard {
      ModifierKeys KeyEvent::modifier = {false,false,false,false,false,false,false,false};
      char KeyEvent::getAsciiKey(void) const {
        if (modifier.shift || modifier.caps_lock) {
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
        case 0x3a: // Capslock
          modifier.caps_lock = !modifier.caps_lock;
          return true;
        case 0x1d: // left control
        case 0xe0: // right control
          modifier.control = true;
          return true;
        case 0x9d: // left and right control break
          modifier.control = false;
          return true;
        case 0x38: // left meta (alt)
          modifier.meta = true;
          return true;
        case 0xb8: // left meta (alt) break
          modifier.meta = false;
          return true;
        case 0x5d: // menu
          modifier.menu = true;
          return true;
        case 0xdd: // menu break
          modifier.menu = false;
          return true;
        case 0x5b:
          modifier.super = true;
          return true;
        case 0xdb:
          modifier.super = false;
          return true;
        default:
          return false;
        }
      }

      KeyEvent::KeyEvent(uint32_t code) : code(code) {
        if(handleModifierKey()) { this->code = 0xFFFFFFFF; }
      }
    }
  }
}
