#include "virtual-console.h"
#include "kernel/drivers/keyboard/key-event.h"
#include "kernel/string/string.h"

namespace kernel {
  /// Is \a key_code a "break" code indicating a key was released?
  /**
     \param key_code[in] is integer in range [0 ... 255].
   */
  inline static bool keyBreakP(uint8_t key_code) {
    return (key_code & 0x80) >> 7;
  }
  void VirtualConsole::handleKey(const drivers::keyboard::KeyEvent* event) {
    if(!keyBreakP(static_cast<uint8_t>(event->getCode()))) {
      switch (event->getCode()) {
      case 0xFFFFFFFF: // Key event was a modifier key.
        break;
      case 0x3b:
        kernel::global::virtual_consoles[0].setCurrent();
        break;
      case 0x3c:
        kernel::global::virtual_consoles[1].setCurrent();
        break;
      case 0x3d:
        kernel::global::virtual_consoles[2].setCurrent();
        break;
      case 0x3e:
        kernel::global::virtual_consoles[3].setCurrent();
        break;
      case 0x3f:
        kernel::global::virtual_consoles[4].setCurrent();
        break;
      case 0x40:
        kernel::global::virtual_consoles[5].setCurrent();
        break;
      case 0x41: // F7
        for(int i = 0; i < 256; i++) {
          VirtualConsole::currentConsole()->put(i, 16);
          VirtualConsole::currentConsole()->put(" ");
        }
        break;
      case 0x42: // F8
        VirtualConsole::currentConsole()->clearInputBuffer();
        break;
      case 0x43: // F9
        VirtualConsole::currentConsole()->clearBuffer();
        break;
      case 0x1c:
        VirtualConsole::currentConsole()->put(VirtualConsole::currentConsole()->getUserInput());
        VirtualConsole::currentConsole()->put("\n");
        break;
      default:
        VirtualConsole::currentConsole()->insertUserInput(static_cast<uint8_t>(event->getAsciiKey()));
      }
      VirtualConsole::currentConsole()->updateInputVideoRam();
    }
    VirtualConsole::currentConsole()->put(static_cast<uint8_t>(event->getCode()), 16);
    VirtualConsole::currentConsole()->put(" ");
    VirtualConsole::currentConsole()->updateOutputVideoRam();
  }
}