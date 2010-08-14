#include "block.h"

namespace kernel {
  namespace memory {
    namespace first_fit {
      void Block::free() {
        allocatedp = false;
      }

      void* Block::alloc(size_t start, uint16_t size) {

        this->start = start & 0x7ffff;
        this->size = size & 0b111111111111;
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

      void* Block::allocIf(uint16_t size) {
        if (!allocatedp && start != 0x0 && this->size >= size) {
          /// \retval "any non null pointer" is a pointer to newly alloated
          /// memory.
          return alloc(this->start, size);
        } else {
          /// \retval (void*)0xFFFFFFFF Pointer to absolute start of
          /// physical memory means nothing was allocated.
          return reinterpret_cast<void*>(0xFFFFFFFF);
        }
      }

      bool Block::brandNewP() {
        if(!allocatedp && start == 0x0 && size == 0x0) {
          return true;
        } else {
          return false;
        }
      }

    }
  }
}
