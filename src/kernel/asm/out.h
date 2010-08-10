#include "types.h"
namespace kernel {
  namespace inlasm {
    /// Output a \a byte to \a port.
    static inline void outb (uint8_t port, uint8_t value);
    /// Output a \a word to \a port.
    static inline void outw (uint8_t port, uint16_t value);
    /// Output a \a word to \a port.
    static inline void outl (uint8_t port, uint32_t value);

  }
}
