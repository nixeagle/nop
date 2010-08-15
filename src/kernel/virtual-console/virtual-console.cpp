#include "virtual-console.h"

namespace kernel {
  void VirtualConsole::Char::clear(void) {
    char_code = 0;
    attributes = 0;
  }
}
