#include "types.h"
namespace c {
  __attribute__((nonnull (1, 2)))
  void *memcpy(void *destination, const void *source, size_t n);
}
