#include "index.h"

namespace kernel {
  namespace memory {
    namespace first_fit {

      using kernel::text_mode::puts;
      using kernel::text_mode::putInteger;
      void Index::free(size_t memory_address) {
        // Look for memory_address in the lookup array.
        for(int i = 0; i < heap_size; i++) {
          if(heap[i].freeIf(memory_address - heap_start)) {
            return;
          }
        }
        asm("int $6;");             // Something went wrong.
      }

      Block* Index::findBrandNewBlock(void) {
        for(int i = 0; i < heap_size; i++) {
          if(heap[i].brandNewP()) {
            return &heap[i];
          }
        }
        // oh crap! out of suitable ram.
        asm("int $4");
      }

      void* Index::malloc(uint16_t size) {
        // First try to allocate in an existing block that is not used.
        for(int i = 0; i < heap_size; i++) {
          kernel::text_mode::put_hex(heap_start, 4, 4);
          void* new_memory = heap[i].allocIf(size);
          if(reinterpret_cast<void*>(0xFFFFFFFF) != new_memory) {
            return new_memory;
          }
        }
        void* new_memory = findBrandNewBlock()->alloc(heap_end, size);
        heap_end += size;

        return reinterpret_cast<void*>(reinterpret_cast<size_t>(new_memory) + heap_start);
      }
    }
  }
}
