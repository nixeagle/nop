#include "index.h"
#include "kernel/panic/kpanic.h"

namespace kernel {
  namespace memory {
    namespace first_fit {

      using kernel::text_mode::puts;
      using kernel::text_mode::putInteger;
      void Index::free(void* memory_address) {
        // Look for memory_address in the lookup array.
        for(int i = 0; i < heap_size; i++) {
          if(heap[i].freeIf(reinterpret_cast<size_t>(memory_address)
                            - heap_start)) {
            return;
          }
        }
        KPANIC("Unable to free a memory block.",
               "Memory deallocation error.");
        //asm("int $6;");             // Something went wrong.
      }

      Block* Index::findBrandNewBlock(void) {
        for(int i = 0; i < heap_size; i++) {
          if(heap[i].brandNewP()) {
            return &heap[i];
          }
        }
        KPANIC("No more free blocks to allocate.",
               "Memory allocation error.");
      }

      void* Index::malloc(uint16_t size) {
        // First try to allocate in an existing block that is not used.
        for(int i = 0; i < heap_size; i++) {
          kernel::text_mode::put_hex(heap_start, 4, 4);
          void* new_memory = heap[i].allocIf(size);
          if(reinterpret_cast<void*>(0xFFFFFFFF) != new_memory) {
            return new_memory + heap_start;
          }
        }
        void* new_memory = findBrandNewBlock()->alloc(heap_end, size);
        kernel::text_mode::put_hex(reinterpret_cast<size_t>(new_memory), 3, 10);
        ;        heap_end += size;

        return reinterpret_cast<void*>(reinterpret_cast<size_t>(new_memory) + heap_start);
      }
    }
  }
}
