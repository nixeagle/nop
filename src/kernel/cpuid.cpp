#include "cpuid.h"

namespace kernel {
  namespace cpuid {
    ProcessorType processorType(void) {
      const u32 eax = cpuid(1).eax();
      // Type is given as bits {13..12} of the eax register.
      switch ((eax & 0x00003000) >> 12) {
      case 0: return ProcessorType::PRIMARY;
      case 1: return ProcessorType::OVERDRIVE;
      case 2: return ProcessorType::SECONDARY;
      case 3: return ProcessorType::RESERVED;
      default: return ProcessorType::INVALID;
      }
    }
  }
}
