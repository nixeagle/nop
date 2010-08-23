#pragma once
#include "types.h"
#include "kernel/text_mode/text_mode.h"

namespace kernel {
  namespace memory {
    namespace first_fit {

      class __attribute__((packed)) Block {
        bool allocatedp : 1;            /// True when block is allocated.
        size_t start : 23;               /// Pointer to start of block.
        uint16_t size : 16;              /// Size in bytes of the block.


      public:
        Block() : allocatedp(false), start(0x0), size(0x00) {};
        /// Mark block as used.
        void* alloc(size_t start, uint16_t size);
        void free(void);            /// Mark block as free.

        /// Conditionally free \ref memory_address.
        bool freeIf(size_t memory_address);
        void* allocIf(uint16_t size);
        bool brandNewP(void);

        void inspect (uint8_t line) {
          kernel::text_mode::puts("Block:", line, 0);
          kernel::text_mode::put_hex(start, line, 10);
          kernel::text_mode::put_hex(size, line, 20);
          kernel::text_mode::put_hex(allocatedp, line, 30);
        }
      };
    }
  }
}
