#pragma once
#include "types.h"
#include "kernel/memory/memory.h"
#include "kernel/string/string.h"

// In theory should be the end of the kernel. This value is set by the
// linker in linker.ld.


inline tacospp::kernel::string::String p(const char* string) __attribute__((always_inline));
inline tacospp::kernel::string::String p(const char* string) {
  return tacospp::kernel::string::String(string);
}

namespace text_mode {
  inline int put_char (char character, unsigned short int line,
                       unsigned short int column);
  int puts(const tacospp::kernel::string::String *string,
           unsigned short int line, unsigned short int column);

  int puts(const tacospp::kernel::string::String &string,
           unsigned short int line, unsigned short int column);

  int put_hex(unsigned int number,unsigned short int line, unsigned short int column);
  char hex2char(unsigned int hex_number);

  /** Clear a line on the console.

      \param[in] line number to clear, should be a value from 0 to
      \ref LINES - 1.

      \post characters on \a line are cleared.
  */
  int clear_line (unsigned short int line);

  /** Makes whole console "blank".

      \post Console screen is cleared of all characters.
  */
  int clear_screen (void);
}


extern "C" void kmain(struct mb_header *header, unsigned int magic);
