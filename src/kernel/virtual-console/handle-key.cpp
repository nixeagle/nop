#include "virtual-console.h"
#include "kernel/drivers/keyboard/key-event.h"

namespace kernel {
  /// Is \a key_code a "break" code indicating a key was released?
  /**
     \param key_code[in] is integer in range [0 ... 255].
   */
  inline static bool keyBreakP(uint8_t key_code) {
    return (key_code & 0x80) >> 7;
  }
  void VirtualConsole::handleKey(void* object,
                                 const drivers::keyboard::KeyEvent* event) {
    if(!keyBreakP(event->getAsciiKey())) {
      reinterpret_cast<VirtualConsole*>(object)->put(static_cast<uint8_t>(event->getAsciiKey()), 16);
      reinterpret_cast<VirtualConsole*>(object)->put(" ");
      reinterpret_cast<VirtualConsole*>(object)->updateOutputVideoRam();
    }
  }
}
