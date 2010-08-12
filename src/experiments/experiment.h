/** Made for messing with concepts

    Stuff here should not stay here after it becomes usable or a "keeper".
*/
#include "types.h"
#include "kernel/memory/memory.h"
#include "kernel/text_mode/text_mode.h"

namespace experiments {
  class Block {
    size_t start;               /// Pointer to start of block.
    uint16_t size;              /// Size in bytes of the block.
    bool allocatedp;            /// True when block is allocated.

  public:
    Block() : start(0x0), size(0xBEEF), allocatedp(false) {};
    /// Mark block as used.
    void* alloc(size_t start, uint16_t size);
    void free(void);            /// Mark block as free.

    /// Conditionally free \ref memory_address.
    bool freeIf(size_t memory_address);
    inline void* allocIf(uint16_t size);
    inline bool brandNewP(void);

    void inspect (uint8_t line) {
      kernel::text_mode::puts("Block:", line, 0);
      kernel::text_mode::put_hex(start, line, 10);
      kernel::text_mode::put_hex(size, line, 20);
      kernel::text_mode::put_hex(allocatedp, line, 30);
    }
  };

  /// Compiletime constant for the maximum objects allowed in our kernel
  /// malloc implementation.... Yes it is primative, but it works.
  static const uint16_t heap_size = 16;
  /** First fit allocater */
  class Heap {

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
    void* malloc(uint16_t size);
    void free(size_t memory_address);
    Heap(void) : heap_start(reinterpret_cast<size_t>(&kernel_end)
                            + kernel::memory::getAllocatedByteCount())
               , heap_end(heap_start) {};
  };
}
