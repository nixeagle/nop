#include "cpuid.h"
#include "kernel/asm/pushfd.h"
#include "kernel/asm/popfd.h"

namespace kernel {
  namespace cpuid {
    /// Test if the unsigned integer x has bit i set or not.
    static inline bool isBitSet(uint x, uint i) {
      return (0 != (x & (1UL << i)));
    }
    bool hasCPUID(void) {
      const u32 old_flags = kernel::as::pushfd();

      // flip the ID flag. If we are able to change this bit to the
      // opposite value, then the processor supports the CPUID
      // instruction.
      kernel::as::popfd(old_flags ^ 0x200000);

      const u32 new_flags = kernel::as::pushfd();

      return old_flags != new_flags;
    }

    inline CpuidResults cpuid(size_t function_code) {
      //static inline void cpuid(int code, dword *a, dword *d) {
      size_t a,b,c,d;
      asm volatile ("cpuid":"=eax"(a),"=edx"(d),"=ebx"(b),"=ecx"(c):
                    "0"(function_code)/*:
                                        "ecx","ebx"*/);
      return CpuidResults(a,b,c,d);
    }

    /// True if cpu is from Intel.
    static inline bool isIntel(void) {
      return (MachineVendor::INTEL == machineVender());
    }
    /// True if cpu is from AMD.
    static inline bool isAMD(void) {
      return (MachineVendor::AMD == machineVender());
    }
    
    ProcessorType processorType(void) {
      static constexpr ProcessorType table[4] = { ProcessorType::PRIMARY,
                                           ProcessorType::OVERDRIVE,
                                           ProcessorType::SECONDARY,
                                           ProcessorType::RESERVED };
      const u32 eax = cpuid(1).eax();
      // Type is given as bits {13..12} of the eax register.
      return table[(eax & 0x00003000) >> 12];
      /*
      switch ((eax & 0x00003000) >> 12) {
      case 0: return ProcessorType::PRIMARY;
      case 1: return ProcessorType::OVERDRIVE;
      case 2: return ProcessorType::SECONDARY;
      case 3: return ProcessorType::RESERVED;
      default: return ProcessorType::INVALID;
      }
      */
    }
    u8 apicId(void) {
      // APIC ID is encoded in bits {31..24}.
      return static_cast<u8>(cpuid(1).ebx() >> 24);
    }
    uint processorStepping(void) {
      return cpuid(1).eax() & 0b1111;
    }
    
    bool hasSSE3(void) {
      return isBitSet(cpuid(1).ecx(), 0);
    }
    // Following three are true if SSE3 is true.
    bool hasMXCSR(void) {
      return hasSSE3();
    }
    bool hasCR4_OSXMMEXCPT(void) {
      return hasSSE3();
    }
    bool hasXF(void) {
      return hasSSE3();
    }
    bool hasFISTTP(void) {
      return hasSSE3() && hasFPU();
    }
    
    bool hasPCMUL(void) {
      return isBitSet(cpuid(1).ecx(), 1);
    }
    bool hasDTES64(void) {
      return isBitSet(cpuid(1).ecx(), 2);
    }

    bool hasMON(void) {
      return isBitSet(cpuid(1).ecx(), 3);
    }
    bool hasDSCPL(void) {
      return isBitSet(cpuid(1).ecx(), 4);
    }

    bool hasVMX(void) {
      
      return (isBitSet(cpuid(1).ecx(), 5) && isIntel());
    }
    bool hasSVM(void) {
      return (isBitSet(cpuid(1).ecx(), 5) && isAMD());
    }

    bool hasSMX(void) {
      return (isBitSet(cpuid(1).ecx(), 6) && isIntel());
    }

    bool hasEST(void) {
      return (isBitSet(cpuid(1).ecx(), 7) && isIntel());
    }

    bool hasTM2(void) {
      return (isBitSet(cpuid(1).ecx(), 8) && isIntel());
    }

    bool hasSSE3(void) {
      return isBitSet(cpuid(1).ecx(), 9);
    }

    bool hasCID(void) {
      return (isBitSet(cpuid(1).ecx(), 10) && isIntel());
    }
    
    // Tests under cpuid(1).edx()
    bool hasFPU(void) {
      return isBitSet(cpuid(1).edx(), 0);
    }
  }
}
