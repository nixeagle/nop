#pragma once
#include "types.h"
#include "block.h"
namespace kernel {
  namespace memory {
    namespace first_fit {

      /** First fit allocater */
      class Index {
        /// Compiletime constant for the maximum objects allowed in our kernel
        /// malloc implementation.... Yes it is primative, but it works.
        static const uint16_t heap_size = 2048;
        size_t heap_start;          /// Starting point for the heap.
        size_t heap_end;
        Block heap[heap_size];
        Block* findBrandNewBlock(void);

      public:
        void inspect() {
          kernel::text_mode::puts("Heap:", 4, 0);
          kernel::text_mode::put_hex(heap_start, 4, 10);
          kernel::text_mode::put_hex(heap_end, 4, 20);

        }
        Block getBlock (int i) { return heap[i];}
        void* __attribute__((optimize ("-O0"),malloc))malloc(uint16_t size);
        void free(void* memory_address);
        Index(void) : heap_start(reinterpret_cast<size_t>(&kernel_end)
                                 + 0x10000
                                 + kernel::memory::getAllocatedByteCount())
                   , heap_end(0x0) {};
      };
    }
  }
}
