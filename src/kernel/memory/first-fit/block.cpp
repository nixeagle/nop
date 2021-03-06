#include "block.h"

namespace kernel {
  namespace memory {
    namespace first_fit {
      void Block::free() {
        allocatedp = false;
      }

      void* Block::alloc(size_t start, size_t size) {

        this->start = start & 0x7fffff;
        this->size = size & 0xffff;
        allocatedp = true;

        /// \retval void* is a pointer to newly allocated memory block.
        return reinterpret_cast<void*>(start);
      }
      bool Block::freeIf(size_t memory_address) {
        kernel::text_mode::put_hex(memory_address, 3, 60);
        kernel::text_mode::put_hex(start, 3, 70);
        if(memory_address == start) {
          free();
          return true;
        } else {
          return false;
        }
      }

      void* Block::allocIf(size_t size) {
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
