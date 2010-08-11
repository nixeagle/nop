#include "experiment.h"
namespace experiments {
  void Block::free() {
    start = 0;
    allocatedp = false;
  }

  void* Block::alloc(size_t start, uint16_t size) {
    this->start = start;
    this->size = size;
    allocatedp = true;
    /// \retval void* is a pointer to newly allocated memory block.
    return reinterpret_cast<void*>(start);
  }
  bool Block::freeIf(size_t memory_address) {
    if(memory_address == start) {
      free();
      return true;
    } else {
      return false;
    }
  }
  void Heap::free(size_t memory_address) {
    // Look for memory_address in the lookup array.
    for(int i = 0; i < 256; i++) {
      if(heap[i].freeIf(memory_address)) {
        return;
      }
    }
    asm("int $3;");             // Something went wrong.
  }
}
