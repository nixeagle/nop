#pragma once
#include "types.h"

namespace kernel {
  namespace drivers {
    namespace keyboard {
      typedef struct {
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
      } ModifierKeys;
      /** Keyboard event */
      class KeyEvent {
        static ModifierKeys modifier;
        /// keysym pressed.
        uint32_t code;

        bool handleModifierKey(void);
      public:
        KeyEvent(uint32_t code);

        uint32_t getCode(void) const { return code; }
        char getAsciiKey(void) const;
      };
    }
  }
}
