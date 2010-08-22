#include "virtual-console.h"
#include "library/c/memory/memcpy.h"
#include "global.h"

namespace kernel {
  // static members
  VirtualConsole* VirtualConsole::current_console = 0;
  VirtualConsole* global::virtual_consoles = 0;
  void VirtualConsole::Char::clear(void) {
    character = 0;
    attributes = 0;
  }

  void VirtualConsole::clearBuffer() {
    for(size_t i = 0; i < COLUMNS * ROWS; i++) {
      output_buffer[i].clear();
    }
  }

  void VirtualConsole::clearInputBuffer() {
    for(size_t i = 0; i < COLUMNS * max_input_height; i++) {
      input_buffer[i].clear();
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

  void VirtualConsole::insertUserInput(uint8_t input) {
    /// @todo Validate input as a printable ASCII char.
    input_buffer[input_cursor++].setChar(input);
  }
  void VirtualConsole::setCurrent () {
    current_console = this;
    kernel::global::key_event = this->handleKey;
  }

  void VirtualConsole::updateOutputVideoRam() {
    Char* videoram = reinterpret_cast<Char*>(VIDEORAM);
    c::memcpy(videoram + (COLUMNS * hud_height), output_buffer,
              sizeof(Char) * COLUMNS
              * (ROWS - hud_height - input_height));
  }
  void VirtualConsole::updateInputVideoRam() {
    Char* videoram = reinterpret_cast<Char*>(VIDEORAM);
    c::memcpy(videoram + COLUMNS * (ROWS - input_height),
              input_buffer,
              sizeof(Char) * COLUMNS * input_height);
  }
}
