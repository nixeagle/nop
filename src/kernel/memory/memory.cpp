#include "memory.h"

namespace kernel {
  namespace memory {
    size_t allocated_byte_count = 0;
    void* kmalloc(size_t size) {
      /// @bug multiplying \ref allocated_byte_count by 2 "works", but
      /// there is no clear reason why this is working.
      void* pointer = reinterpret_cast<void*>(&kernel_end + 2 * allocated_byte_count);
      allocated_byte_count += size;
      return pointer;
    }
    size_t getAllocatedByteCount() {
      return allocated_byte_count;
    }
  }
}
