#pragma once
#include "types.h"

namespace kernel {
  namespace drivers {
    namespace keyboard {

      /// \author duckinator - taken from dux
      const unsigned char keysym_us[128] =
        {
          0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
          '9', '0', '-', '=', '\x08',  /* Backspace */
          '\t',        /* Tab */
          'q', 'w', 'e', 'r',    /* 19 */
          't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',    /* Enter key */
          0,              /* 29 - Control */
          'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  /* 39 */
          '\'', '`', 0,            /* Left shift */
          '\\', 'z', 'x', 'c', 'v', 'b', 'n',      /* 49 */
          'm', ',', '.', '/', 0,        /* Right shift */
          '*',
          0,  /* Alt */
          ' ',  /* Space bar */
          0,  /* Caps lock */
          0,  /* 59 - F1 key ... > */
          0, 0, 0, 0, 0, 0, 0, 0,
          0,  /* < ... F10 */
          0,  /* 69 - Num lock*/
          0,  /* Scroll Lock */
          0,  /* Home key */
          0,  /* Up Arrow */
          0,  /* Page Up */
          '-',
          0,  /* Left Arrow */
          0,
          0,  /* Right Arrow */
          '+',
          0,  /* 79 - End key*/
          0,  /* Down Arrow */
          0,  /* Page Down */
          0,  /* Insert Key */
          0,  /* Delete Key */
          0, 0, 0,
          0,  /* F11 Key */
          0,  /* F12 Key */
          0,  /* All other keys are undefined */
        };

      /// \author duckinator - taken from dux
      const unsigned char keysym_us_shift[128] =
        {
          0, 27, '!', '@', '#', '$', '%', '^', '&', '*',    /* 9 */
          '(', ')', '_', '+', '\x08',  /* Backspace */
          '\t',        /* Tab */
          'Q', 'W', 'E', 'R',    /* 19 */
          'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',    /* Enter key */
          0,              /* 29 - Control */
          'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',  /* 39 */
          '"', '~', 0,            /* Left shift */
          '|', 'Z', 'X', 'C', 'V', 'B', 'N',      /* 49 */
          'M', '<', '>', '?', 0,        /* Right shift */
          '*',
          0,  /* Alt */
          ' ',  /* Space bar */
          0,  /* Caps lock */
          0,  /* 59 - F1 key ... > */
          0, 0, 0, 0, 0, 0, 0, 0,
          0,  /* < ... F10 */
          0,  /* 69 - Num lock*/
          0,  /* Scroll Lock */
          0,  /* Home key */
          0,  /* Up Arrow */
          0,  /* Page Up */
          '-',
          0,  /* Left Arrow */
          0,
          0,  /* Right Arrow */
          '+',
          0,  /* 79 - End key*/
          0,  /* Down Arrow */
          0,  /* Page Down */
          0,  /* Insert Key */
          0,  /* Delete Key */
          0, 0, 0,
          0,  /* F11 Key */
          0,  /* F12 Key */
          0,  /* All other keys are undefined */
        };


      enum KeyCaps : unsigned char {
        kError, kEscape, k1, k2, k3, k4, k5, k6, k7, k8, k9, k0,
          kMinus, kEqual, kBackSpace, kTab, kq, kw, ke, kr, kt,
          ky, ku, ki, ko, kp, kLeftSquare, kRightSquare, kEnter,
          kLeftControl, ka, ks, kd, kf, kg, kh, kj, kk, kl, kSemiColon,
          kSingleQuote, kBackTick, kLeftShift, kForwardSlash, kz,
          kx, kc, kv, kb, kn, km, kComma, kPeriod, kBackSlash, kRightShift,
          kKeyPadModifier, kLeftAlt, kSpaceBar, kCapsLock,
          kF1, kF2, kF3, kF4, kF5, kF6, kF7, kF8, kF9, kF10, kNumLock,
          kScrollLock, kKeyPad7, kKeyPad8, kKeyPad9, kKeyPadMinus,
          kKeyPad4, kKeyPad5, kKeyPad6, kKeyPadPlus,
          kKeyPad1, kKeyPad2, kKeyPad3, kKeyPad0, kKeyPadPeriod,
          kAltSysRq, kUncommon1, kUnlabeledKey, kF11, kF12
          // The remainder are not as well specified, so we do not try to
          // map them to names.
          };
    }
  }
}
