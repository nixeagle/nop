#pragma once
#include "types.h"
#include "kernel/string/string.h"
#include "kernel/memory/memory.h"

namespace kernel {
  namespace drivers {
    namespace keyboard {
      class KeyEvent;           // Forward declaration.
    }
  }

  class VirtualConsole {
    class Char {
      uint8_t character;
      uint8_t attributes;
    public:
      /// Zero out memory associated with char.
      void clear(void);
      inline void setAttributes(uint8_t attributes) { this->attributes = attributes; }
      inline void setChar(unsigned char character) {
        this->character = character;
        this->attributes = 0x07;
      }
      Char() : character(0), attributes(0) {};
    };
    /// Pointer to the currently active console.
    static VirtualConsole* current_console;
    static uint8_t console_count;
    /// Max number of columns a virtual console can have.
    const static uint8_t COLUMNS = 80;

    /// Max number of rows a virtual console can have.
    const static uint8_t ROWS = 25;

    const static size_t VIDEORAM = 0xb8000;
    /// No copy constructor
		/// \param no_copy[in] test
    //    VirtualConsole(const VirtualConsole& no_copy) = delete;
    /// No assignment operator.
    //    VirtualConsole& operator=(VirtualConsole no_assignment) = delete;
    // Sure this is a good idea to have it non const/static? As is we
    // require dynamic memory management, might work if we allocate
    // batches of say 25 rows of memory instead of just one. The allocater
    // is still pretty weak, stressing it with lots of small objects might
    // not be too nice for it.
    /// Number of rows to have for scrollback, maybe 100?
    /** Note that currently displaying items count as scrollback as a copy
        of it must be maintained at all times for scrolling to work as
        scrolling means being able to go forward and backwards ;)

        Currently made constant.
    */
    const static uint16_t scrollback_rows = 500;

    /// Height of the heads up display.
    /** If the rest of the virtual console scrolls, text in this display
        remains "constant"
    */
    const static uint16_t hud_height = 4;

    /// Maximum number of rows a user's input can cover.
    /** This is currently static, maybe later this should become dynamic
     */
    const static uint16_t max_input_height = 5;

    /// Location for printed output
    Char* output_buffer;

    /// Location for user to input commands
    Char* input_buffer;

    uint32_t output_cursor; /// Position of marker for text entry.

    uint32_t input_cursor; /// User's input cursor, where text appears.

    /// Height of input area
    /** Should default to 1, but expanding is possible if user requires
        more then one line of input to enter a command.

        @todo Compute this from the current (non zero) length of the input buffer)
    */
    uint8_t input_height;

    /// Pointer to start of last visible row
    uint32_t visible_buffer_bottom_row;
  public:

    VirtualConsole(void)
      : output_buffer(reinterpret_cast<Char*>(kernel::memory::flat_kmalloc(sizeof(Char) * COLUMNS * ROWS)))
      , input_buffer(reinterpret_cast<Char*>(kernel::memory::flat_kmalloc(sizeof(Char) * COLUMNS * max_input_height)))
      , output_cursor(0)
      , input_cursor(0)
      , input_height(1) {
      this->clearBuffer();
      this->clearInputBuffer();
    };
    static VirtualConsole* currentConsole(void) {
      return VirtualConsole::current_console;
    }
    static void setCurrentConsole (VirtualConsole* console) {
      current_console = console;
    }

    void setCurrent (void);
    kernel::string::String* getUserInput(void) const;
    /// Insert a char to the user input buffer
    /** \param input[in] A printable ASCII character.

        \pre \ref input_buffer must have space for one character.

        \post A character is added to the \ref input_buffer and \ref
        input_cursor is incremented by one.
    */
    void insertUserInput(uint8_t input);
    // scrolling
    void scrollUp(uint16_t rows);
    void scrollDown(uint16_t rows);
    void scrollToCursor(void);  /// Scroll to where current cursor is.
    void scrollToBottom(void);  /// Scroll to very bottom of buffer.
    void scrollToTop(void);     /// Scroll to very top of buffer.
    void clearBuffer(void);     /// Clear scrollback buffer.
    void clearInputBuffer(void);

    // hud
    void showHud(void); /// Shows the hud, this will cover up some of the buffer.
    void hideHud(void); /// Hide the hud, shows more of the scrollback.

    // inserting things to buffer. (non user input/hud portions)
    void put(const kernel::string::String* string); /// pascal strings
    void put(const char* string);  /// C strings, must terminate in null.
    void put(uint32_t integer, uint8_t base = 10);    /// Unsigned integer.
    //    void put(int integer);         /// Print a possibly negative integer.
    void put(const void* pointer); /// Pointer/address to something.
    void putc(char character);      /// A single character.

    // Generic visible buffer clearing.
    void clearLine(uint8_t line); /// Clear a specific line.

    // updating display ram
    void updateOutputVideoRam(void);
    void updateInputVideoRam(void);

    // Inserting user's key inputs
    static void handleKey(const drivers::keyboard::KeyEvent* event);
  };
  namespace global {
    extern VirtualConsole* virtual_consoles;
  }
}
