#include "cpuid.h"
#include "kernel/asm/pushfd.h"
#include "kernel/asm/popfd.h"
#include "kernel/panic/kpanic.h"

namespace kernel {
  namespace cpuid {
    static bool hasCPUID(void) {
      const u32 old_flags = kernel::as::pushfd();

      // flip the ID flag. If we are able to change this bit to the
      // opposite value, then the processor supports the CPUID
      // instruction.
      kernel::as::popfd(old_flags ^ 0x200000);

      const u32 new_flags = kernel::as::pushfd();

      return old_flags != new_flags;
    }

    static MachineVendor machineVendor (uint edx) {
      switch (edx) {
      case 0x49656E69: // 'ineI' for Intel
        return MachineVendor::INTEL;
      case 0x69746E65: // 'enti' for AuthenticAMD
        return MachineVendor::AMD;
      case 0x48727561: // 'aurH' (short of CentaurHauls) for VIA.
        return MachineVendor::VIA; 
      default:
        return MachineVendor::UNKNOWN;
      }
    }
      
    CpuInfo::CpuInfo (void) {
      _has_cpuid = kernel::cpuid::hasCPUID();
      if (!_has_cpuid) { // @todo try to detect as much as possible anyway
        return;
      }
      u8 max_function_code;
      {
        const CpuidResults result = cpuid(0);
        max_function_code = static_cast<u8>(result.eax());
        _machine_vendor = machineVendor(result.edx());
      }
      if (1 > max_function_code) { return; }
      {
        const CpuidResults result = cpuid(1);
        fn0000_0001_eax(result.eax());
        _features_ecx = result.ecx();
        _features_edx = result.edx();
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
        _family = static_cast<u8>(base_family + extended_family);
        // And model is defined as {ExtendedModel[3:0],BaseModel[3:0]}.
        _model = static_cast<u8>((extended_model << 4) + base_model);
      } else { // We treat extended_family as reserved and ignore it.
        _family = base_family;
        _model  = base_model;
      }
      _stepping = (0xF & eax);

      if (vendor() == MachineVendor::INTEL) {
        static constexpr ProcessorType table[4] = {ProcessorType::PRIMARY,
                                                   ProcessorType::OVERDRIVE,
                                                   ProcessorType::SECONDARY,
                                                   ProcessorType::RESERVED};
        // Type is given as bits {13..12} of the eax register.
        _processor_type = table[(eax & 0x00003000) >> 12];
      } else { // CPU Vendor does not use these bits for anything.
        _processor_type = ProcessorType::UNUSED_BY_VENDOR;
      }
      return;
    }
    void CpuInfo::fn0000_0001_ebx(uint ebx) {
      _local_apic_id = static_cast<u8>((0xFF & (ebx >> 24)));
      _logical_processor_count = (0xFF & (ebx >> 16));
      _clflush_size = (0xFF & (ebx >> 8));
      _brand_id = (0xFF & ebx);
      return;
    }
    //static CpuInfo initial_cpu;

    

    inline bool isBitSet(uint x, uint i) {
      return (0 != (x & (1UL << i)));
    }

    CpuidResults cpuid(size_t function_code) {
      //static inline void cpuid(int code, dword *a, dword *d) {
      size_t a,b,c,d;
      asm volatile ("cpuid":"=eax"(a),"=edx"(d),"=ebx"(b),"=ecx"(c):
                    "0"(function_code)/*:
                                        "ecx","ebx"*/);
      return CpuidResults(a,b,c,d);
    }
    namespace {
      inline bool isIntel(const CpuInfo* const cpu) {
        return (MachineVendor::INTEL == (*cpu).vendor());
      }
      inline bool isAMD(const CpuInfo* const cpu) {
        return (MachineVendor::AMD == cpu->vendor());
      }
    }

    
    bool CpuInfo::hasSSE3(void) const {
      return isBitSet(_features_ecx, 0);
    }
    
    // Following three are true if SSE3 is true.
    bool CpuInfo::hasMXCSR(void) const {
      return hasSSE3();
    }
    bool CpuInfo::hasCR4_OSXMMEXCPT(void) const {
      return hasSSE3();
    }
    bool CpuInfo::hasXF(void) const {
      return hasSSE3();
    }
    bool CpuInfo::hasFISTTP(void) const {
      return hasSSE3() && hasFPU();
    }
    
    bool CpuInfo::hasPCLMUL(void) const {
      return isBitSet(_features_ecx, 1);
    }
    bool CpuInfo::hasDTES64(void) const {
      return isBitSet(_features_ecx, 2);
    }

    bool CpuInfo::hasMON(void) const {
      return isBitSet(_features_ecx, 3);
    }
    bool CpuInfo::hasDSCPL(void) const {
      return isBitSet(_features_ecx, 4);
    }

    bool CpuInfo::hasVMX(void) const {
      return (isBitSet(_features_ecx, 5));
    }
    bool CpuInfo::hasSVM(void) const {
      return (isBitSet(_features_ecx, 5));
    }

    bool CpuInfo::hasSMX(void) const {
      return (isBitSet(_features_ecx, 6));
    }

    bool CpuInfo::hasEST(void) const {
      return (isBitSet(_features_ecx, 7));
    }

    bool CpuInfo::hasTM2(void) const {
      return (isBitSet(_features_ecx, 8));
    }

    bool CpuInfo::hasSSSE3(void) const {
      return isBitSet(_features_ecx, 9);
    }

    bool CpuInfo::hasCID(void) const {
      return (isBitSet(_features_ecx, 10));
    }

    bool CpuInfo::hasFMA(void) const {
      return (isBitSet(_features_ecx, 12));
    }
    bool CpuInfo::hasFMA3(void) const {
      return (isBitSet(_features_ecx, 12) && isIntel(this));
    }
    bool CpuInfo::hasFMA4(void) const {
      return (isBitSet(_features_ecx, 12) && isAMD(this));
    }

    bool CpuInfo::hasCX16(void) const {
      return (isBitSet(_features_ecx, 13));
    }

    bool CpuInfo::hasETPRD(void) const {
      return (isBitSet(_features_ecx, 14));
    }

    bool CpuInfo::hasPDCM(void) const {
      return (isBitSet(_features_ecx, 15));
    }

    // Tests under cpuid(1).edx()
    bool CpuInfo::hasFPU(void) const {
      return isBitSet(_features_edx, 0);
    }
  }
}
