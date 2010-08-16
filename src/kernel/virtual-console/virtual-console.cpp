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



  void VirtualConsole::putc(char character) {
    switch(character) {
    case '\n':
      output_cursor += COLUMNS - output_cursor % COLUMNS;
      break;
    default:
      output_buffer[output_cursor++].setChar(character);
    }
  }




  void VirtualConsole::updateOutputVideoRam() {
    Char* videoram = reinterpret_cast<Char*>(VIDEORAM);
    c::memcpy(videoram + (COLUMNS * hud_height), output_buffer,
              sizeof(Char) * COLUMNS * (ROWS - hud_height));
  }
}
