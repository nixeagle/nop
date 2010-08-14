#include "memcpy.h"

namespace c {
  void *memcpy(void *destination, const void *source, size_t n) {
    for(int i = 0; i < n; i++) {
      destination[i] = source[i];
    }
    return destination;
  }
}
