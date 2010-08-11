/** Made for messing with concepts

    Stuff here should not stay here after it becomes usable or a "keeper".
*/
#include "types.h"
#include "kernel/memory/memory.h"
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
    inline bool freeIf(size_t memory_address);
  };

  /** First fit allocater */
  class Heap {
    size_t heap_start;          /// Starting point for the heap.
    Block heap[256];
  public:
    void* malloc(uint16_t size);
    void free(size_t memory_address);
    Heap() : heap_start(kernel_end + 0x2000) {};
  };
}
