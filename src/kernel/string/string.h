#pragma once
#include "types.h"

namespace tacospp {
  namespace kernel {
    namespace string {
      size_t strlen(const char* string) __attribute__((fastcall));
      class String {
      public:
        String(const char* string)
          : length(strlen(string)),
            string(string)
        {

        }
        /// @todo Make accessor function for the length.
        size_t length;
        const char* string;
      };
      // string make_string(const char* string);

      //      Length of a string, C and pascal styles respectively.

      inline size_t strlen(const String* string) {
        return string->length;
      }
    }
  }
}
