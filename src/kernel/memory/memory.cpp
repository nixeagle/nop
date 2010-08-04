#include "memory.h"

namespace kernel {
  namespace memory {
    size_t allocated_byte_count = 0;
    void* kmalloc(size_t size) {
      void* pointer = reinterpret_cast<void*>(&kernel_end + allocated_byte_count);
      allocated_byte_count += size;
      return pointer;
    }
    size_t getAllocatedByteCount() {
      return allocated_byte_count;
    }
  }
}
