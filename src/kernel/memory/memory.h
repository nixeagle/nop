#pragma once
#ifndef _MEMORY_H_
#define _MEMORY_H_
#include "types.h"

extern "C" int kernel_end;

namespace kernel {
  namespace memory {
    /** Allocate memory off the end of the kernel.

        \warning This kmalloc has no equivalent free function. All memory
        allocated cannot be reclaimed by the kernel!.

        \param[in] size of requested memory in bytes.

        \pre Enough memory to satisfy request for \a size bytes.

        \post \li \ref allocated_byte_count is incremented by \a size.
        \post \li System has allocated dynamic memory memory.
     */
    void* kmalloc(size_t size)
      __attribute__((alloc_size(1),fastcall,malloc,warn_unused_result,warning ("depr"), optimize ("-O0")));
    void* flat_kmalloc(size_t size);
    size_t getAllocatedByteCount(void);
    void init(void);
  }
}

#endif /* _MEMORY_H_ */


