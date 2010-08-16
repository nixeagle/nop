#include "virtual-console.h"
#include "library/c/memory/memcpy.h"
namespace kernel {
  // static members
  VirtualConsole* VirtualConsole::current_console = 0;

  void VirtualConsole::Char::clear(void) {
    character = 0;
    attributes = 0;
  }

  void VirtualConsole::clearBuffer() {
    for(size_t i = 0; i < COLUMNS * ROWS; i++) {
      output_buffer[i].clear();
    }
  }

  // Put overloads
  void VirtualConsole::put(const char* string) {
    while('\0' != *string) {
      put(*string++);
    }
  }

  void VirtualConsole::put(char character) {
    output_buffer[output_cursor++].setChar(character);
  }

  void VirtualConsole::updateOutputVideoRam() {
    void* videoram = reinterpret_cast<void*>(VIDEORAM);
    c::memcpy(videoram, output_buffer, sizeof(Char) * COLUMNS * ROWS);
  }
}
