#pragma once
#include "types.h"
#include "kernel/string/string.h"

// In theory should be the end of the kernel. This value is set by the
// linker in linker.ld.
extern "C" int kernel_end;

namespace std {
  namespace string {
    struct string {
      size_t length;
      const char* string;
    };
    string pascal_string(const char* string);
  }
}

namespace c {
  namespace string {
    size_t strlen(const char *string);
    inline size_t strlen(const std::string::string *string);
  }
}

inline tacospp::kernel::string::String p(const char* string) __attribute__((always_inline));
inline tacospp::kernel::string::String p(const char* string) {
  return tacospp::kernel::string::String(string);
}
void* kmalloc(size_t size);

namespace text_mode {
  inline int put_char (char character, unsigned short int line,
                       unsigned short int column);
  int puts(const tacospp::kernel::string::String *string,
           unsigned short int line, unsigned short int column);

  int puts(const tacospp::kernel::string::String &string,
           unsigned short int line, unsigned short int column);
  int clear_line (unsigned short int line);
  int clear_screen (void);
}


extern "C" void kmain(struct mb_header *header, unsigned int magic);
