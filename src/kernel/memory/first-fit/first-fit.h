#include "types.h"

namespace kernel {
  namespace memory {
    namespace first_fit {
      /// Compiletime constant for the maximum objects allowed in our kernel
      /// malloc implementation.... Yes it is primative, but it works.
      const uint16_t heap_size = 2048;

    }
  }
}
