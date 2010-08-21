#include "global.h"

namespace kernel {
  namespace global {
    void (*key_event)(const drivers::keyboard::KeyEvent*) =  0;
  }
}
