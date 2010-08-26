#include "text_mode.h"
#include "kernel/panic/kpanic.h"

namespace kernel {
  /** Initial bootup text mode manipulation.

      This covers the graphics, and printing to the primitive console that
      we get after the kernel gets control from the bootloader.
  */
  namespace text_mode {
    /// Address to the start of videoram
    const unsigned int VIDEORAM = 0xb8000;
    /// Number of character rows in the console.
    const unsigned short int LINES = 25;
    /// Number of characters that fit on a \ref LINES.
    const unsigned short int COLUMNS = 80;

    int put_char (char character, unsigned int line,
                         unsigned int column) {
      unsigned char *videoram = reinterpret_cast<unsigned char *> (VIDEORAM);

      if(line >= LINES || column >= COLUMNS) {
        KPANIC("Attempt to place a char on a non existing column or row."
               , "Out of bounds");
      } else {                  // A character goes in every _other_ byte.
        size_t offset = (COLUMNS << 1) * line + (column << 1);
        videoram[offset] = character;
        /// @todo Needs to select forground and background colors by setting
        /// the second byte in video ram to something other then 0x07.
        videoram[offset + 1] = 0x07;
      }
      return 0; /// \suc0
    }


    int clear_line (unsigned short int line) {
      if (LINES < line) {
        KPANIC("Attempt to clear a line that does not eixist, eg larger then LINES."
               , "Clear line");
      } else {
        for(unsigned short int i = 0; i < COLUMNS; i++) {
          put_char(' ', line, i);
        }
        return 0; /// \suc0
      }
    }


    int clear_screen () {
      for (unsigned short int line = 0; line < LINES; line++) {
        clear_line(line);
      }
      return 0; /// \suc0
    }

    // int puts(const tacospp::kernel::string::String *string,
    //            unsigned short int line, unsigned short int column) {
    //   for(size_t i = 0; i < string->length; i++) {
    //     put_char(string->string[i],line, static_cast<unsigned short int>(i + column));
    //   }
    //   return 0;
    // }

    /// @todo Figure out some way to get rid of this instance of pass by
    /// reference, but we have to do it right now to comply with the C++
    /// standard.
    int puts(const kernel::string::String &string,
             unsigned int line, unsigned int column) {
      for(size_t i = 0; i < string.length(); i++) {
        put_char(string.string[i],line, i + column);
      }
      return 0;
    }

    int puts(const char* string, unsigned int line,
             unsigned int column) {
      for(size_t i = 0; string[i] != '\0'; i++) {
        put_char(string[i], line, i + column);
      }
      return 0;
    }
    char hex2char(unsigned int hex_number) {
      if (0xa > hex_number) {
        return static_cast<char>(hex_number | 0x30);
      } else if (0x10 > hex_number) {
        return static_cast<char>(hex_number + 55);
      } else {
        KPANIC("Given hex number was greater then 0xa, so can't fit in base 16.",
               "Converting a hex number to a char failed.");
      }
    }
    int put_hex(unsigned int number, unsigned int line, unsigned int column) {
      /// @todo Needs to be made 64bit compatible.
      /// mask needs to be changed depending on the target.
      for(unsigned int mask = 0xF0000000, shift = 28;
          mask > 0;
          mask = mask >> 4, column++, shift -= 4) {
        put_char(hex2char((number & mask) >> shift), line, column);
      }
      return 0; /// \suc0
    }

    uint32_t putInteger(uint32_t number, uint8_t base, uint32_t line,
                    uint32_t column) {
      /// @bug When number is initially 0 we print nothing. We should be
      /// printing '0'.
      if(0 < number) {
        uint32_t col = putInteger(number/base, base, line, column);
        // digit (and base) is 36 or less, no danger of overflow.
        uint8_t digit = static_cast<uint8_t>(number % base);
        if(10 > digit) {
          // digit (9) + '0' (48) = '9' (57) is < 256, no overflow.
          put_char(static_cast<uint8_t>(digit + '0'), line, col);
        } else if (36 > digit) {
          // digit (35) - 10 + 'A' (65) = 'Z' (90) < 256
          put_char(static_cast<uint8_t>(digit - 10 + 'A'), line, col);
        } else {
          KPANIC("digit >= 36: No way to print base 37 and up!",
                 "Integer overflow");
        }
        return col + 1;
      }
      return column;
    }
  }
}
