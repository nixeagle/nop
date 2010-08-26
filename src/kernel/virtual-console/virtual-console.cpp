#include "virtual-console.h"
#include "library/c/memory/memcpy.h"
#include "global.h"
#include "kernel/string/string.h"
namespace kernel {
  // static members
  VirtualConsole* VirtualConsole::current_console = 0;
  VirtualConsole* global::virtual_consoles = 0;
  VirtualConsole* _testout = 0;
  VirtualConsole& global::testout = *_testout;
  VirtualConsole& global::kout = *global::virtual_consoles;
  void VirtualConsole::Char::clear(void) {
    character = 0;
    attributes = 0;
  }

  void VirtualConsole::clearBuffer() {
    for(size_t i = 0; i < COLUMNS * scrollback_rows; i++) {
      output_buffer[i].clear();
    }
    output_cursor = 0; // Set cursor back to start.
  }

  void VirtualConsole::clearInputBuffer() {
    for(size_t i = 0; i < COLUMNS * max_input_height; i++) {
      input_buffer[i].clear();
    }
    input_cursor = 0; // Set cursor back to start.
  }

  void VirtualConsole::putc(char character) {
    switch(character) {
    case '\n':
      output_cursor += COLUMNS - output_cursor % COLUMNS;
      break;
    default:
      output_buffer[output_cursor++].setChar(character);
    }
    if(output_scroll_cursor/COLUMNS == output_cursor/COLUMNS) {
      output_scroll_cursor += COLUMNS;
    }
  }

  void VirtualConsole::insertUserInput(uint8_t input) {
    /// @todo Validate input as a printable ASCII char.
    input_buffer[input_cursor++].setChar(input);
  }
  void VirtualConsole::setCurrent () {
    global::kout = *this;
    current_console = this;
    kernel::global::key_event = this->handleKey;
  }

  void VirtualConsole::updateOutputVideoRam() {
    Char* videoram = reinterpret_cast<Char*>(VIDEORAM);
    c::memcpy(videoram + (COLUMNS * kHUD_HEIGHT),
              output_buffer - (COLUMNS * outputHeight()) + output_scroll_cursor,
              sizeof(Char) * COLUMNS * outputHeight());
  }
  void VirtualConsole::updateInputVideoRam() {
    Char* videoram = reinterpret_cast<Char*>(VIDEORAM);
    c::memcpy(videoram + COLUMNS * (ROWS - input_height),
              input_buffer,
              sizeof(Char) * COLUMNS * input_height);
  }

  uint16_t VirtualConsole::charBufferLength(const Char* buffer) {
    uint16_t i = 0;
    while (buffer[i].nullp()) {
      i++;
    }
    return i;
  }
  kernel::string::String* VirtualConsole::getUserInput(void) const {
    using string::String;

    String *string = new String(charBufferLength(input_buffer));
    for(uint16_t i = 0; i < string->length(); i++) {
      (*string)[i] = input_buffer[i].getCharacter();
    }
    return string; /// \retval String* is copied string from input.
  }
}
