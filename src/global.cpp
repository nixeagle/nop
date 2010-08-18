#include "global.h"

namespace kernel {
  namespace global {
    void (*key_event)(void*, const drivers::keyboard::KeyEvent*) =  0;
  }
}
