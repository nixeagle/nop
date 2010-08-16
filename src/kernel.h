#pragma once
#include "types.h"
#include "kernel/string/string.h"

inline kernel::string::String p(const char* string) __attribute__((always_inline));
inline kernel::string::String p(const char* string) {
  return kernel::string::String(string);
}


/// Main function for kernel entry from the loader.
extern "C" void kmain(struct mb_header *header, unsigned int magic);

// Mostly "internal" functions just to help with debugging
/// Put current allocated memory to upper right corner of console.
void puts_allocated_memory(void);

/// Loop and print out current point in loop to console.
void busy_loop(size_t limit);
