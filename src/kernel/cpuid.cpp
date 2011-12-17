#include "cpuid.h"
#include "kernel/asm/pushfd.h"
#include "kernel/asm/popfd.h"

namespace kernel {
  namespace cpuid {
    bool hasCPUID(void) {
      const u32 old_flags = kernel::as::pushfd();

      // flip the ID flag. If we are able to change this bit to the
      // opposite value, then the processor supports the CPUID
      // instruction.
      kernel::as::popfd(old_flags ^ 0x200000);

      const u32 new_flags = kernel::as::pushfd();

      return old_flags != new_flags;
    }
    
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

    uint processorStepping(void) {
      return cpuid(1).eax() & 0b1111;
    }
  }
}
