#pragma once
/// CPUID for x86 machines.

#ifdef __x86_64
#elif __i386
#else
#error "CPUID include is meant for x86 computers only!"
#endif

#include "types.h"

namespace kernel {
  namespace cpuid {

    /// Represents all important results from a CPUID instruction.
    class CpuidResults {
    public:
      CpuidResults(size_t ax, size_t bx, size_t cx, size_t dx) :
        _ax(ax), _bx(bx), _cx(cx), _dx(dx) { }
      // Following functions mask off the low bits to get the specifc parts
      // of the various registers that are needed.
      u32 eax(void) const { return _ax & 0xFFFFFFFF; }
      u32 ebx(void) const { return _bx & 0xFFFFFFFF; }
      u32 ecx(void) const { return _cx & 0xFFFFFFFF; }
      u32 edx(void) const { return _dx & 0xFFFFFFFF; }
      u16 ax(void)  const { return _ax & 0xFFFF;     }
      u16 bx(void)  const { return _bx & 0xFFFF;     }
      u16 cx(void)  const { return _cx & 0xFFFF;     }
      u16 dx(void)  const { return _dx & 0xFFFF;     }
      u8  al(void)  const { return _ax & 0xFF;       }
      u8  bl(void)  const { return _bx & 0xFF;       }
      u8  cl(void)  const { return _cx & 0xFF;       }
      u8  dl(void)  const { return _dx & 0xFF;       }

    private:
      const size_t _ax;
      const size_t _bx;
      const size_t _cx;
      const size_t _dx;
    };

    /// API interface to the CPUID instruction.
    /// @TODO Cache results to prevent excessive CPUID calls.
    inline CpuidResults cpuid(size_t function_code) {
      //static inline void cpuid(int code, dword *a, dword *d) {
      size_t a,b,c,d;
      asm volatile ("cpuid":"=eax"(a),"=edx"(d),"=ebx"(b),"=ecx"(c):
                    "0"(function_code)/*:
                                        "ecx","ebx"*/);
      return CpuidResults(a,b,c,d);
    }

    /// Enumerates CPU makers.
    /// @TODO Add other CPU venders.
    enum class MachineVendor {
      INTEL,
      AMD,
      VIA,
      UNKNOWN
    };

    inline MachineVendor machineVendor (void) {
      switch (cpuid(0).edx()) {
      case 0x696E6549: // 'ineI' for Intel
        return MachineVendor::INTEL;
      case 0x65967449: // 'enti' for AuthenticAMD
        return MachineVendor::AMD;
      case 0x56494120: // 'VIA ' (hoping this is it...)
        return MachineVendor::VIA; 
      default:
        return MachineVendor::UNKNOWN;
      }
    }

    /// How many standard function calls are supported on this CPU?
    /// Basically if CPUID is supported, we know at leasat code=0 is
    /// supported. This function tells us what the upper bound of the
    /// supported range is.
    inline uint upperFunctionCode(void) {
      return cpuid(0).eax();
    }

    /// Type and or 'role' of the process. Most computers have only one
    /// PRIMARY processor.
    enum class ProcessorType {
      PRIMARY,
      OVERDRIVE,
      SECONDARY,
      RESERVED,
      INVALID
    };

    ProcessorType processorType(void);

    /// Processor specific values.length
    /// @range [0 ... 15]
    uint processorStepping(void);
    
    /// Does this processor support HyperThreading?
    inline bool isHyperThreading(void);

    
  }
}
