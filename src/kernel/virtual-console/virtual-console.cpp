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
      putc(*string++);
    }
  }

  void VirtualConsole::putc(char character) {
    output_buffer[output_cursor++].setChar(character);
  }

  void VirtualConsole::put(const void* pointer) {
    put("0x");
    put(reinterpret_cast<size_t>(pointer), 16);
  }

  void VirtualConsole::put(uint32_t number, uint8_t base) {
    /// \todo base being larger then 36 ought to cause an error instead of
    /// being silently ignored.
    if(0 < number && 36 >= base) {
      put(number/base);
      // digit can't be larger then 36.
      unsigned char digit = static_cast<uint8_t>(number % base);
      // It is not possible for digit to be larger then 91
      if(10 > digit) {
        putc(static_cast<uint8_t>(digit + '0'));
      } else {
        putc(static_cast<uint8_t>(digit - 10 + 'A'));
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
    void* videoram = reinterpret_cast<void*>(VIDEORAM);
    c::memcpy(videoram, output_buffer, sizeof(Char) * COLUMNS * ROWS);
  }
}
