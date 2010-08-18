#include "virtual-console.h"
#include "library/c/memory/memcpy.h"
#include "global.h"
#include "kernel/drivers/keyboard/key-event.h"
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

  void VirtualConsole::clearInputBuffer() {
    for(size_t i = 0; i < max_input_height; i++) {
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

  void VirtualConsole::setCurrent () {
    current_console = this;
    kernel::global::key_event = this->handleKey;
  }

  void VirtualConsole::handleKey(void* object,
                                 const drivers::keyboard::KeyEvent* event) {
    reinterpret_cast<VirtualConsole*>(object)->put(static_cast<uint8_t>(event->getAsciiKey()), 16);
    reinterpret_cast<VirtualConsole*>(object)->put(" ");
    reinterpret_cast<VirtualConsole*>(object)->updateOutputVideoRam();
  }

  void VirtualConsole::updateOutputVideoRam() {
    Char* videoram = reinterpret_cast<Char*>(VIDEORAM);
    c::memcpy(videoram + (COLUMNS * hud_height), output_buffer,
              sizeof(Char) * COLUMNS * (ROWS - hud_height));
  }
}
