#pragma once
#ifndef _MEMORY_H_
#define _MEMORY_H_
#include "types.h"

extern "C" int kernel_end;

namespace kernel {
  namespace memory {
    void* __attribute__((alloc_size(1))) kmalloc(size_t size);
    size_t getAllocatedByteCount(void);
  }
}

#endif /* _MEMORY_H_ */


