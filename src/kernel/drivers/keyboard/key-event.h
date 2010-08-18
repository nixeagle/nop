#pragma once
#include "types.h"

namespace kernel {
  namespace drivers {
    namespace keyboard {

      /** Keyboard event */
      class KeyEvent {
        static struct {
          /// Was shift pressed?
          /// \pre Assumes only one keyboard is attached.
          bool shift : 1;
          /// \pre Assumes only one keyboard is attached.
          /// Was control pressed?
          bool control : 1;
          bool caps_lock : 1;
          bool num_lock : 1;
          bool meta : 1;
          bool super : 1;
          bool menu : 1;
          bool scroll_lock : 1;
        } modifier;
        /// keysym pressed.
        uint32_t code;
      public:
        KeyEvent(uint32_t code)
          : code(code) {}
        char getAsciiKey(void) const {
          return static_cast<char>(code);
        }
      };
    }
  }
}
