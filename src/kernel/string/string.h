#pragma once
#include "types.h"

namespace kernel {
  namespace string {

    class String {
    public:
      String(const char* string)
        : _length(length(string)),
          string(string) { }
      static size_t length(const char* string) {
        size_t length = 0;
        while('\0' != string[length]) {
          length++;
        }
        return length;
      }

      inline size_t length() const { return _length; };
      inline static size_t length(const String* string) {
        return string->length();
      }

    private:
      /// @todo Make accessor function for the length.
      size_t _length;
    public:
      const char* string;
    };
    // string make_string(const char* string);

    //      Length of a string, C and pascal styles respectively.


  }
}

