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
    VirtualConsole& out = *VirtualConsole::currentConsole();
    if(!keyBreakP(static_cast<uint8_t>(event->getCode()))) {
      switch (event->getCode()) {
      case 0xFFFFFFFF: // Key event was a modifier key.
        break;
      case 0x3b ... 0x40:
        kernel::global::virtual_consoles[event->getCode() - 0x3b].setCurrent();
        out = *VirtualConsole::currentConsole();
        break;
      case 0x41: // F7
        for(int i = 0; i < 256; i++) {
          out.put(i, 16);
          out.put(" ");
        }
        break;
      case 0x43: // F9
        out.clearBuffer();
        break;
      case 0x4B: // left
        //
        break;
      case 0x48: // up
        break;
      case 0x50: // down
        break;
      case 0x4D: // right
        break;
      case 0x1c:
        {
          string::String* input = out.getUserInput();
          out.put(input);
          delete input;
          out.put("\n");
          out.clearInputBuffer();
        }
        break;
      default:
        out.insertUserInput(static_cast<uint8_t>(event->getAsciiKey()));
        out.updateInputVideoRam();
        break;
      }
      out.updateInputVideoRam();
    }
    out.updateOutputVideoRam();
  }
}
