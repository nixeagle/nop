/** Global variables for the whole nop kernel.

    Sadly in an operating systems some things just have to be global to
    the whole operating state such as function hooks.

    \file global.h
*/

#pragma once
namespace kernel {
  namespace drivers {
    namespace keyboard {
      class KeyEvent;           // Forward declaration.
    }
  }
  class VirtualConsole;
  namespace global {
    void (*key_event)(drivers::keyboard::KeyEvent);
  }
}

