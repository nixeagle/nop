#pragma once
#include "types.h"

namespace kernel {
  namespace drivers {
    namespace keyboard {

      /** Keyboard event */
      class KeyEvent {
        /// Was shift pressed?
        bool shift;
        /// Was control pressed?
        bool control;
        /// keysym pressed.
        uint32_t code;
        KeyEvent() : shift(false), control(false), code(0) {}
      };
    }
  }
}
