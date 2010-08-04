#pragma once

// Indicates pointer size depending on which arch is in use.
#ifdef __x86_64
// 8 bytes
  typedef unsigned long int size_t;
#elif __i386
// 4 bytes
  typedef unsigned int size_t;
#endif

// I think these are the same on 32bit and 64 bit x86 machines...
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;

#ifdef __x86_64
typedef unsigned long int uint64_t;
#endif

#ifdef __x86_64
#elif __i386
#endif
