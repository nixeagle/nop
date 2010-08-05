#pragma once
#include "types.h"
#include "kernel/memory/memory.h"
#include "kernel/string/string.h"

// In theory should be the end of the kernel. This value is set by the
// linker in linker.ld.


inline tacospp::kernel::string::String p(const char* string) __attribute__((always_inline));
inline tacospp::kernel::string::String p(const char* string) {
  return tacospp::kernel::string::String(string);
}


extern "C" void kmain(struct mb_header *header, unsigned int magic);
