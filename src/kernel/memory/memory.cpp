#include "memory.h"
#include "types.h"
#include "first-fit/index.h"

namespace kernel {
  namespace memory {
    size_t allocated_byte_count = 0;
    static int alloc_count = 0;

    first_fit::Index heap = first_fit::Index();
    void init() {}
    void* flat_kmalloc(size_t size) {
      alloc_count++;
      void* pointer = reinterpret_cast<void*>(reinterpret_cast<size_t>(&kernel_end) + allocated_byte_count);
      allocated_byte_count += size;
      return pointer;
    }
    extern void* kmalloc(size_t size) {

      kernel::text_mode::putInteger(size, 10, 7, 40);
      void* pointer = heap.malloc(size);
      kernel::text_mode::put_hex((size_t)pointer, 21, 70);

      //heap.inspect();

      return pointer;
    }
    size_t getAllocatedByteCount() {
      heap.inspect();
      heap.getBlock(0).inspect(17);
      heap.getBlock(1).inspect(18);;
      heap.getBlock(2).inspect(19);;
      kernel::text_mode::putInteger(alloc_count, 10, 22, 70);
      return allocated_byte_count;
    }
  }
}
