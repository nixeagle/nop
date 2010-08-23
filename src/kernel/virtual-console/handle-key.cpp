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
      case 0x3b:
        kernel::global::virtual_consoles[0].setCurrent();
        out = *VirtualConsole::currentConsole();
        break;
      case 0x3c:
        kernel::global::virtual_consoles[1].setCurrent();
        out = *VirtualConsole::currentConsole();
        break;
      case 0x3d:
        kernel::global::virtual_consoles[2].setCurrent();
        out = *VirtualConsole::currentConsole();
        break;
      case 0x3e:
        kernel::global::virtual_consoles[3].setCurrent();
        out = *VirtualConsole::currentConsole();
        break;
      case 0x3f:
        kernel::global::virtual_consoles[4].setCurrent();
        out = *VirtualConsole::currentConsole();
        break;
      case 0x40:
        kernel::global::virtual_consoles[5].setCurrent();
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
        out.put(out.getUserInput());
        out.put("\n");
        out.put(out.output_cursor);
        out.put(" -- ");
        out.put(out.output_scroll_cursor);
        out.put(" ::");
        out.clearInputBuffer();
        break;
      default:
        out.insertUserInput(static_cast<uint8_t>(event->getAsciiKey()));
        out.updateInputVideoRam();
      }
      out.updateInputVideoRam();
    }
    out.put(static_cast<uint8_t>(event->getCode()), 16);
    out.put(" ");
    out.updateOutputVideoRam();
  }
}
