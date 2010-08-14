#include "types.h"
#include "kernel/string/string.h"
namespace kernel {
  class VirtualConsole {
    static VirtualConsole* current_console;
    const static uint8_t columns = 80;
    const static uint8_t rows = 25;

    uint16_t cursor; /// Position of marker for text entry.

    uint8_t input_cursor; /// User's input cursor, where text appears.

    /// Height of the heads up display.
    /** If the rest of the virtual console scrolls, text in this display
        remains "constant"
    */
    uint8_t hud_height;
    /// Height of input area
    /** Should default to 1, but expanding is possible if user requires
        more then one line of input to enter a command.
    */
    uint8_t input_height;

    // Sure this is a good idea to have it non const/static? As is we
    // require dynamic memory management, might work if we allocate
    // batches of say 25 rows of memory instead of just one. The allocater
    // is still pretty weak, stressing it with lots of small objects might
    // not be too nice for it.
    /// Number of rows to have for scrollback, maybe 100?
    /** Note that currently displaying items count as scrollback as a copy
        of it must be maintained at all times for scrolling to work as
        scrolling means being able to go forward and backwards ;)
    */
    uint16_t scrollback_rows;

    uint16_t visible_buffer_bottom_row; /// Last visible row on screen.
  public:
    VirtualConsole(void)
      : cursor(0), input_height(1), scrollback_rows(500) {};
    kernel::string::String* getUserInput(void) const;
    void clearUserInput(void);
    // scrolling
    void scrollUp(uint16_t rows);
    void scrollDown(uint16_t rows);
    void scrollToBottom(void);  /// Scroll to very bottom of buffer.
    void scrollToTop(void);     /// Scroll to very top of buffer.
    void clearBuffer(void);     /// Clear scrollback buffer.

    // hud
    void showHud(void);    /// Shows the hud, this will cover up some of the buffer.
    void hideHud(void);    /// Hide the hud, shows more of the scrollback.

    // inserting things to buffer. (non user input/hud portions)
    void put(const kernel::string::String* string); /// pascal strings
    void put(const char* string); /// C strings, must terminate in null.
    void put(uint32_t integer); /// Unsigned integer.
    void put(int integer);/// Print a possibly negative integer.
    void put(const void* pointer); /// Pointer/address to something.
    void put(char character); /// A single character.

    // Generic visible buffer clearing.
    void clearLine(uint8_t line); ///Clear a specific line.
  };
}
