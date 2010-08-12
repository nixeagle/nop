#pragma once
#include "types.h"

namespace kernel {
  namespace drivers {
    namespace keyboard {
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
