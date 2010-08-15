#include "memcpy.h"

namespace c {
  void *memcpy(void *destination, const void *source, size_t n) {
    char* temp_dest = reinterpret_cast<char*>(destination);
    char* temp_source = reinterpret_cast<char*>(const_cast<void*>(source));
    for(size_t i = 0; i < n; i++) {
      *temp_dest++ = *temp_source++;
    }
    /// \retval pointer to destination
    return destination;
  }
}
