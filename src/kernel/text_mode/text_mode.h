#include "kernel/string/string.h"
namespace kernel {
  namespace text_mode {
    inline int put_char (char character, unsigned short int line,
                         unsigned short int column);
    // int puts(const tacospp::kernel::string::String *string,
    //          unsigned short int line, unsigned short int column);

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
}
