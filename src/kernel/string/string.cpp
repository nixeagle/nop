#include "string.h"

namespace tacospp {
  namespace kernel {
    namespace string {
      size_t strlen(const char* string) {
        size_t length = 0;
        while('\0' != string[length]) {
          length++;
        }
        return length;
      }
    }
  }
}
