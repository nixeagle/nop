#pragma once
/** Global variables for the whole nop kernel.

    Sadly in an operating systems some things just have to be global to
    the whole operating state such as function hooks.

    \file global.h
*/


namespace kernel {
  namespace drivers {
    namespace keyboard {
      class KeyEvent;           // Forward declaration.
    }
  }
  class VirtualConsole;
  namespace global {
    /// Function pionter to handle keyboard events.
    /** Is in charge of doing everything related to handling a keybaord
        event, our current use will be to place keyboard input on the
        screen and handle primative "commands" by recognizing a newline to
        mean end of command.

       \param object[in,out] is a pointer to any instance capable of holding
       event state or otherwise making use of keyboard input.

       \param event[in] is a
       \ref kernel::drivers::keyboard::KeyEvent "KeyEvent" struct.

       \todo Change this to something better able to handle objects, maybe
       a functor or using a form of delagates. I (nixeagle) was looking at
       http://www.codeproject.com/KB/cpp/FastDelegate.aspx earlier today
       [2010-08-18 Wed 04:21] and the public domain header files there
       look pretty useful.

       \pre \a object is instantiated and prepared for handling input.

       \post \a object's state may be modified in the course of handling a
       keyboard event, the modifications should be restricted to only
       those needed to handle a keyboard event.

       \todo Make this pure by not modifying object, instead returning the
       modiifcations vie the return value. This will not work so well as
       long as object is the whole virtual console and not just some
       smaller part of it.
     */
    extern void (*key_event)(void* object,
                             const drivers::keyboard::KeyEvent* event);
  }
}

