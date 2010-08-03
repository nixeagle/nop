#pragma once

// Indicates pointer size depending on which arch is in use.
#ifdef __x86_64
// 8 bytes
  typedef unsigned long int size_t;
#elif __i386
// 4 bytes
  typedef unsigned int size_t;
#endif


