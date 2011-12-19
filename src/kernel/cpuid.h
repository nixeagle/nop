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
      inline u32 eax(void) const { return _ax & 0xFFFFFFFF; }
      inline u32 ebx(void) const { return _bx & 0xFFFFFFFF; }
      inline u32 ecx(void) const { return _cx & 0xFFFFFFFF; }
      inline u32 edx(void) const { return _dx & 0xFFFFFFFF; }
      inline u16 ax(void)  const { return _ax & 0xFFFF;     }
      inline u16 bx(void)  const { return _bx & 0xFFFF;     }
      inline u16 cx(void)  const { return _cx & 0xFFFF;     }
      inline u16 dx(void)  const { return _dx & 0xFFFF;     }
      inline u8  al(void)  const { return _ax & 0xFF;       }
      inline u8  bl(void)  const { return _bx & 0xFF;       }
      inline u8  cl(void)  const { return _cx & 0xFF;       }
      inline u8  dl(void)  const { return _dx & 0xFF;       }

    private:
      const size_t _ax;
      const size_t _bx;
      const size_t _cx;
      const size_t _dx;
    };

    /// API interface to the CPUID instruction.
    /// @TODO Cache results to prevent excessive CPUID calls.
    __attribute__((noinline)) CpuidResults cpuid(size_t function_code);

    /// True if the processor supports the CPUID instruction.
    bool hasCPUID(void);

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

    u8 clflushChunckCount(void);
    u8 logicalCpuCount(void);
    u8 apicId(void);

    /// True if the SSE3 extensions are supported.
    bool hasSSE3(void);
    /// True if MXCSR is supported.
    bool hasMXCSR(void);
    /// True if CR4.OSXMMEXCPT is supported.
    bool hasCR4_OSXMMEXCPT(void);
    /// True if #XF is supported.
    bool hasXF(void);
    /// True if FISTTP is supported.
    bool hasFISTTP(void);

    /// True if the PCLMULQDQ instruction is supported.
    bool hasPCLMUL(void);

    /// True if cpu supports 64 bit Debug Trace and EMON store MSRs.
    bool hasDTES64(void);

    /// Cpu supports the MONITOR and MWAIT instructions.
    /// Additionally as a direct consequence MISC_ENABLE_MONE,
    /// MISC_ENABLE_LCMV, MONITOR_FILTER_LINE_SIZE MSR.
    bool hasMON(void);
    
    /// Indicates that a CPL qualified debug store is supported.
    /// @note No known source actually tells us what a CPL qualified debug
    /// store even is! Sandpile and wikipedia both have a very short
    /// summary with no real good elaboration. CPUID docs from intel and
    /// AMD both seem to skip over the issue. Processor documentation
    /// probably has it, but a quick search over 4000+ pages did not turn
    /// up anything.
    bool hasDSCPL(void);

    /// True when Virtual Machine Extensions are supported.
    /// This is an Intel extension and is not binary compatable with AMD's.
    /// Enabled instructions on Intel are:
    /// CR4.VMPTRLD, VMPTRST, VMCLEAR, VMREAD, VMWRITE, VMLAUNCH, VMRESUME,
    /// VMXOFF, VMXON, INVEPT, INVVPID, VMCALL, VMFUNC.
    bool hasVMX(void);
    
    /// True when Secure Virtual Machine instructions are supported.
    /// This is an AMD extension and is not binary compatable with Intel's.
    /// Enabled instructions are:
    /// CR4.VMPTRLD, CLGI, INVLPGA, SKINIT, STGI, VMLOAD, VMMCALL, VMRUN,
    /// VMSAVE.
    bool hasSVM(void);
    
    /// Are Safer Mode Extensions, e.g. the GETSEC instruction enabled?
    /// Chip has to be an Intel. When available, CR4.SMXE can be used to
    /// turn on and off this feature.
    bool hasSMX(void);

    /// Enhanced Intel SpeedStep Technology is supported.
    /// Means that IA32_PERF_STS and IA32_PERF_CTL registers are
    /// implemented.
    bool hasEST(void);

    /// Processor has the Thermal Monitor 2 control circuit.
    /// Means the following are available:
    /// THERM_INTERRUPT, THERM_STATUS MSRs, 
    /// THERM2_CONTROL MSR, xAPIC thermal LVT entry,
    /// @note This is an Intel only extension. AMD's CPUID reference manual
    /// makes no mention of this.
    bool hasTM2(void);

    /// SSSE3 instructions are supported.
    bool hasSSSE3(void);
    
    bool hasCID(void);
    bool hasFMA(void);
    bool hasCX16(void);
    bool hasETPRD(void);
    bool hasPDCM(void);
    bool hasPCID(void);
    bool hasDCA(void);
    bool hasSSE4_1(void);
    bool hasSSE4_2(void);
    bool hasX2APIC(void);
    bool hasMOVBE(void);
    bool hasPOPCNT(void);
    bool hasTSCD(void);
    bool hasAES(void);
    bool hasXSAVE(void);
    bool hasOSXSAVE(void);
    bool hasAVX(void);
    bool hasF16C(void);
    bool hasRDRAND(void);

    bool hasFPU(void);
    bool hasVME(void);
    bool hasDE(void);
    bool hasPSE(void);
    bool hasTSC(void);
    bool hasMSR(void);
    bool hasPAE(void);
    bool hasCX8(void);
    bool hasAPIC(void);
    bool hasSEP(void);
    bool hasMTRR(void);
    bool hasPGE(void);
    bool hasMCA(void);
    bool hasCMOV(void);
    bool hasPAT(void);
    bool hasPSE36(void);
    /// Processor Serial Number
    /// Disabled on more recent processors due to privacy issues.
    bool hasPSN(void);
    bool hasCLFL(void);
    bool hasDTES(void);
    bool hasACPI(void);
    bool hasMMX(void);
    bool hasFXSR(void);
    bool hasSSE(void);
    bool hasSSE2(void);
    bool hasSS(void);

    /// Does this processor support HyperThreading?
    bool hasHTT(void);
    bool hasTM1(void);
    bool hasIA64(void);
    bool hasPBE(void);

  }
}
