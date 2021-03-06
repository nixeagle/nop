#pragma once
#ifndef _MEMORY_H_
#define _MEMORY_H_
#include "types.h"

extern "C" int kernel_end;

namespace kernel {
  namespace memory {
    extern void* kmalloc(size_t size)
      __attribute__((alloc_size(1),fastcall,malloc,warn_unused_result, optimize ("-O0")));
    /** Allocate memory off the end of the kernel.

        \warning This kmalloc has no equivalent free function. All memory
        allocated cannot be reclaimed by the kernel!.

        \param[in] size of requested memory in bytes.

        \pre Enough memory to satisfy request for \a size bytes.

        \post \li \ref allocated_byte_count is incremented by \a size.
        \post \li System has allocated dynamic memory memory.
     */
    void* flat_kmalloc(size_t size);
    size_t getAllocatedByteCount(void);
    void init(void);
    void kfree(void *p);
  }
}

inline void *operator new(size_t size) {
  return kernel::memory::kmalloc(size);
}

inline void *operator new[](size_t size) {
  return kernel::memory::kmalloc(size);
}

inline void operator delete(void *p) {
  kernel::memory::kfree(p);
}

inline void operator delete[](void *p) {
  kernel::memory::kfree(p);
}

#endif /* _MEMORY_H_ */


