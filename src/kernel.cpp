#include "kernel.h"
#include "kernel/idt/idt.h"
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

  char hex2char(unsigned int hex_number) {
    if (0xa > hex_number) {
        return static_cast<char>(hex_number | 0x30);
    } else if (0x10 > hex_number) {
        return static_cast<char>(hex_number + 55);
    } else {
      // Failing a proper conversion, for now return the character 'Z'.  Z
      // is an invalid hex "number", so seeing it means an error happened
      // somewhere.
      return 'Z';
    }
  }
  int put_hex(unsigned int number, unsigned short int line, unsigned short int column) {
    for(unsigned int mask = 0xF0000000, shift = 28;
        mask > 0;
        mask = mask >> 4, column++, shift -= 4) {
      put_char(hex2char((number & mask) >> shift), line, column);
    }
    return 0;
  }
}

extern "C" void kmain(struct mb_header *header, unsigned int magic) {
  // Setup memory:
  text_mode::clear_screen();

  if(0x2BADB002 != magic) {
    text_mode::puts(p("ERROR: Bootloader magic does not match."), 15, 20);
    text_mode::put_hex(magic,16,22);
  }

  kernel::idt::init(255);
  text_mode::put_hex(0x1234afeb,6,3);
  text_mode::put_hex((unsigned int)&kernel_end,8,9);
  text_mode::puts(p("Tacospp"), 0, 0);

  kernel::memory::kmalloc(4);

  text_mode::put_hex(kernel::memory::getAllocatedByteCount(),10,9);
  text_mode::put_hex((unsigned int)kernel_end,11,9);

  text_mode::put_hex(sizeof(long int), 0, 30);
  // for(unsigned short int i = 0, j = 1; i < 80; i++, j++) {
  //   text_mode::put_char('H', 0, ++i);
  //   text_mode::put_char('H', 0, ++j);
  // }

  for(unsigned int i = 0;; i++) {
    text_mode::put_hex(i, 5,30);
    for(unsigned int i = 0; i < 10000000;) {
      i++;
    }
  }
  return;
}
