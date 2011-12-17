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
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t; // TODO: Are we sure?
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;

#ifdef __x86_64
typedef unsigned long int uint64_t;
typedef signed long long int int64_t;
typedef unsigned uint64_t u64;
typedef signed int64_t s64;
#endif

#ifdef __x86_64
#elif __i386
#endif

// Define some nice shorthand typenames in terms of the C++11 types.
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

/// Short for an unsigned machine word.
typedef unsigned long int uint;
/// Short for a signed machine word.
typedef unsigned long int sint;
