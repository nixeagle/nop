#include "kernel.h"
#include "types.h"

namespace text_mode {
  const int VIDEORAM = 0xb8000;
  const unsigned short int LINES = 25;
  const unsigned short int COLUMNS = 80;

  inline int put_char (char character, unsigned short int line,
                       unsigned short int column) {
    unsigned char *videoram = reinterpret_cast<unsigned char *> (VIDEORAM);

    if(line >= LINES || column >= COLUMNS) {
      return 1;                 // Inputs are too big.
    }

    {                           // A character goes in every _other_ byte.
      size_t offset = (COLUMNS << 1) * line + (column << 1);
      videoram[offset] = character;
      //BUGBUG: Needs to select forground and background colors.
      videoram[offset + 1] = 0x07;
    }
    return 0;
  }

  int clear_line (unsigned short int line) {
    if (LINES <= line) {
      return 1;                 // Inputs are too big.
    } else {
      for(unsigned short int i = 0; i < COLUMNS; i++) {
        put_char(' ', line, i);
      }
      return 0;
    }
  }

  int clear_screen () {
    for (unsigned short int line = 0; line < LINES; line++) {
      clear_line(line);
    }
    return 0;
}

  int puts(const tacospp::kernel::string::String *string,
             unsigned short int line, unsigned short int column) {
    for(size_t i = 0; i < string->length; i++) {
      put_char(string->string[i],line, static_cast<unsigned short int>(i + column));
    }
    return 0;
  }

  int puts(const tacospp::kernel::string::String &string,
             unsigned short int line, unsigned short int column) {
    for(size_t i = 0; i < string.length; i++) {
      put_char(string.string[i],line, static_cast<unsigned short int>(i + column));
    }
    return 0;
  }

}

void* kmalloc(size_t size) {
  void* pointer = reinterpret_cast<void*>(kernel_end);
  kernel_end += size;
  return pointer;
}

extern "C" void kmain(struct mb_header *header, unsigned int magic) {
  text_mode::clear_screen();

  if(0x1BADB001 != magic) {
    text_mode::puts(p("ERROR: Bootloader magic does not match."), 15, 20);
  }

  text_mode::puts(p("Tacospp"), 0, 0);
  // for(unsigned short int i = 0, j = 1; i < 80; i++, j++) {
  //   text_mode::put_char('H', 0, ++i);
  //   text_mode::put_char('H', 0, ++j);
  // }


  return;
}
