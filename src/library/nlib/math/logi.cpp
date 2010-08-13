#include "logi.h"

namespace nib {
  namespace math {
    unsigned int logi(unsigned int number, unsigned int base) {
      unsigned int i = 1;
      while(base <= number) {
        number /= base;
        i++;
      }
      return i;
    }
  }
}
