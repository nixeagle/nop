#pragma once
#include "kernel/string/string.h"
namespace kernel {
  namespace text_mode {
    int put_char (char character, unsigned int line,
                         unsigned int column);
    // int puts(const tacospp::kernel::string::String *string,
    //          unsigned short int line, unsigned short int column);

    int puts(const kernel::string::String &string,
             unsigned int line, unsigned int column);

    /** Put a C string to console.

        \note This exists only to make putting literal strings to the
        console easier as postfix literal prefixes are not yet in gcc
        (specified in draft C++0x standard).
     */
    int puts(const char* string, unsigned int line,
             unsigned int column);
    int put_hex(unsigned int number,unsigned int line, unsigned int column);
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
}
