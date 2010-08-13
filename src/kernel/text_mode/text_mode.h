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
    /** Put any \a number in \a base to console.

        \param number[in] to output.
        \param base[in] of number to output. Should be in range [2 ... 36].
        \param line[in] to print on.
        \param column[in] to start on.
    */
    uint32_t putInteger(uint32_t number, uint8_t base, uint32_t line,
                    uint32_t column);
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
