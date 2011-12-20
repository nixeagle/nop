#include "cpuid.h"
#include "kernel/asm/pushfd.h"
#include "kernel/asm/popfd.h"

namespace kernel {
  namespace cpuid {

      static MachineVendor machineVendor (uint edx) {
        switch (edx) {
        case 0x696E6549: // 'ineI' for Intel
          return MachineVendor::INTEL;
        case 0x656E7469: // 'enti' for AuthenticAMD
          return MachineVendor::AMD;
        case 0x61757248: // 'aurH' (short of CentaurHauls) for VIA.
          return MachineVendor::VIA; 
        default:
          return MachineVendor::UNKNOWN;
        }
      }
      
      CpuInfo::CpuInfo (void) {
        has_cpuid = hasCPUID();
        if (!has_cpuid) { return ; }
        u8 max_function_code;
        {
          const CpuidResults result = cpuid(0);
          max_function_code = static_cast<u8>(result.eax());
          machine_vendor = machineVendor(result.edx());
        }
        if (1 > max_function_code) { return; }
        {
          const CpuidResults result = cpuid(1);
          fn0000_0001_eax(result.eax());
          misc_features_ecx = result.ecx();
          misc_features_edx = result.edx();
          fn0000_0001_ebx(result.ebx());
        }
      }
      void CpuInfo::fn0000_0001_eax(uint eax) {
        // We want to get bits [11:8].
        const u8 base_family = static_cast<u8>((0x700 & eax) >> 8);
        const u8 base_model = static_cast<u8>((0x70 & eax) >> 4);

        // Test if extended family and model are supported.
        if (0xF == base_family) { 
          const u8 extended_family = static_cast<u8>((0x0FF00000 & eax) >> 20);
          const u8 extended_model = static_cast<u8>((0x000F0000 & eax) >> 16);
        
          // CPUID documentation from AMD says to do this. There is an
          // implicit assumption that extended_family will not be larger
          // than 0xf0. 
          family = static_cast<u8>(base_family + extended_family);
          // And model is defined as {ExtendedModel[3:0],BaseModel[3:0]}.
          model = static_cast<u8>((extended_model << 4) + base_model);
        } else { // We treat extended_family as reserved and ignore it.
          family = base_family;
          model  = base_model;
        }
        stepping = (0xF & eax);

        if (machine_vendor == MachineVendor::INTEL) {
          static constexpr ProcessorType table[4] = {ProcessorType::PRIMARY,
                                                     ProcessorType::OVERDRIVE,
                                                     ProcessorType::SECONDARY,
                                                     ProcessorType::RESERVED};
          processor_type = table[(eax & 0x00003000) >> 12];
        } else { // CPU Vendor does not use these bits for anything.
          processor_type = ProcessorType::UNUSED_BY_VENDOR;
        }
        return;
      }
      void CpuInfo::fn0000_0001_ebx(uint ebx) {
        local_apic_id = static_cast<u8>((0xFF & (ebx >> 24)));
        logical_processor_count = (0xFF & (ebx >> 16));
        clflush_size = (0xFF & (ebx >> 8));
        brand_id = (0xFF & ebx);
        return;
      }
      static CpuInfo initial_cpu;

    

    inline bool isBitSet(uint x, uint i) {
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

    CpuidResults cpuid(size_t function_code) {
      //static inline void cpuid(int code, dword *a, dword *d) {
      size_t a,b,c,d;
      asm volatile ("cpuid":"=eax"(a),"=edx"(d),"=ebx"(b),"=ecx"(c):
                    "0"(function_code)/*:
                                        "ecx","ebx"*/);
      return CpuidResults(a,b,c,d);
    }

    
    MachineVendor machineVendor (void) {
      return initial_cpu.machine_vendor;
    }

    /// True if cpu is from Intel.
    static inline bool isIntel(void) {
      return (MachineVendor::INTEL == initial_cpu.machine_vendor);
    }
    /// True if cpu is from AMD.
    static inline bool isAMD(void) {
      return (MachineVendor::AMD == initial_cpu.machine_vendor);
    }
    
    ProcessorType processorType(void) {
      // Type is given as bits {13..12} of the eax register.
      return initial_cpu.processor_type;
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
    
    bool hasPCLMUL(void) {
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

    bool hasSSSE3(void) {
      return isBitSet(cpuid(1).ecx(), 9);
    }

    bool hasCID(void) {
      return (isBitSet(cpuid(1).ecx(), 10) && isIntel());
    }

    bool hasFMA(void) {
      return (isBitSet(cpuid(1).ecx(), 12));
    }
    bool hasFMA3(void) {
      return (isBitSet(cpuid(1).ecx(), 12) && isIntel());
    }
    bool hasFMA4(void) {
      return (isBitSet(cpuid(1).ecx(), 12) && isAMD());
    }

    bool hasCX16(void) {
      return (isBitSet(cpuid(1).ecx(), 13));
    }

    bool hasETPRD(void) {
      return (isBitSet(cpuid(1).ecx(), 14) && isIntel());
    }
    // Tests under cpuid(1).edx()
    bool hasFPU(void) {
      return isBitSet(cpuid(1).edx(), 0);
    }
  }
}
