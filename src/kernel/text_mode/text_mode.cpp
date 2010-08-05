#include "text_mode.h"

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

    inline int put_char (char character, unsigned short int line,
                         unsigned short int column) {
      unsigned char *videoram = reinterpret_cast<unsigned char *> (VIDEORAM);

      if(line >= LINES || column >= COLUMNS) {
        /// \retval 1 Input \a line is larger then \ref LINES or
        /// input \a column is larger then \ref COLUMNS.
        return 1;
      }

      {                           // A character goes in every _other_ byte.
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
        /// \retval 1 Input \a line is larger then \ref LINES
        return 1;
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
        /// \retval 'Z' means an unknown error occurred somewhere.
        return 'Z';
      }
    }
    int put_hex(unsigned int number, unsigned short int line, unsigned short int column) {
      /// @todo Needs to be made 64bit compatible.
      /// mask needs to be changed depending on the target.
      for(unsigned int mask = 0xF0000000, shift = 28;
          mask > 0;
          mask = mask >> 4, column++, shift -= 4) {
        put_char(hex2char((number & mask) >> shift), line, column);
      }
      return 0; /// \suc0
    }
  }
}
