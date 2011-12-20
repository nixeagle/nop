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

    /// Enumerates CPU makers.
    /// @TODO Add other CPU venders.
    enum class MachineVendor {
      INTEL,
      AMD,
      VIA,
      UNKNOWN,
      UNINITIALIZED
    };


    /// How many standard function calls are supported on this CPU?
    /// Basically if CPUID is supported, we know at least code=0 is
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
      INVALID,
      UNUSED_BY_VENDOR
    };

    class CpuInfo {
    public:
      CpuInfo(void);

      bool hasCPUID(void) const { return _has_cpuid; }
      /// Who does the processor claim to be made by?
      MachineVendor vendor(void) const { return _machine_vendor; }
      ProcessorType type(void) const { return _processor_type; }
      u8 apicId(void) const { return _local_apic_id; }

      /// Processor specific values.length
      /// @range [0 ... 15]
      u8 stepping(void) const { return _stepping & 0xF; }



      /// True if the SSE3 extensions are supported.
      bool hasSSE3(void) const;
      /// True if MXCSR is supported.
      bool hasMXCSR(void) const;
      /// True if CR4.OSXMMEXCPT is supported.
      bool hasCR4_OSXMMEXCPT(void) const;
      /// True if #XF is supported.
      bool hasXF(void) const;
      /// True if FISTTP is supported.
      bool hasFISTTP(void) const;

      /// True if the PCLMULQDQ instruction is supported.
      bool hasPCLMUL(void) const;

      /// True if cpu supports 64 bit Debug Trace and EMON store MSRs.
      bool hasDTES64(void) const;

      /// Cpu supports the MONITOR and MWAIT instructions.
      /// Additionally as a direct consequence MISC_ENABLE_MONE,
      /// MISC_ENABLE_LCMV, MONITOR_FILTER_LINE_SIZE MSR.
      bool hasMON(void) const;
    
      /// Indicates that a CPL qualified debug store is supported.
      /// @note No known source actually tells us what a CPL qualified debug
      /// store even is! Sandpile and wikipedia both have a very short
      /// summary with no real good elaboration. CPUID docs from intel and
      /// AMD both seem to skip over the issue. Processor documentation
      /// probably has it, but a quick search over 4000+ pages did not turn
      /// up anything.
      bool hasDSCPL(void) const;

      /// True when Virtual Machine Extensions are supported.
      /// This is an Intel extension and is not binary compatable with AMD's.
      /// Enabled instructions on Intel are:
      /// CR4.VMPTRLD, VMPTRST, VMCLEAR, VMREAD, VMWRITE, VMLAUNCH, VMRESUME,
      /// VMXOFF, VMXON, INVEPT, INVVPID, VMCALL, VMFUNC.
      bool hasVMX(void) const;
    
      /// True when Secure Virtual Machine instructions are supported.
      /// This is an AMD extension and is not binary compatable with Intel's.
      /// Enabled instructions are:
      /// CR4.VMPTRLD, CLGI, INVLPGA, SKINIT, STGI, VMLOAD, VMMCALL, VMRUN,
      /// VMSAVE.
      bool hasSVM(void) const;
    
      /// Are Safer Mode Extensions, e.g. the GETSEC instruction enabled?
      /// Chip has to be an Intel. When available, CR4.SMXE can be used to
      /// turn on and off this feature.
      bool hasSMX(void) const;

      /// Enhanced Intel SpeedStep Technology is supported.
      /// Means that IA32_PERF_STS and IA32_PERF_CTL registers are
      /// implemented.
      bool hasEST(void) const;

      /// Processor has the Thermal Monitor 2 control circuit.
      /// Means the following are available:
      /// THERM_INTERRUPT, THERM_STATUS MSRs, 
      /// THERM2_CONTROL MSR, xAPIC thermal LVT entry,
      /// @note This is an Intel only extension. AMD's CPUID reference manual
      /// makes no mention of this.
      bool hasTM2(void) const;

      /// SSSE3 instructions are supported.
      bool hasSSSE3(void) const;

      /// Processor allows L1 data cache to be set as adaptive or shared.
      /// Context ID is toggled with MISC_ENABLE.L1DCCM.
      /// @note only known to be supported on some Intel processors.
      bool hasCID(void) const;

      /// Has Fused Multiply Add support.
      /// @note Intel processors support FMA3 while AMD supports FMA4. It is
      /// not yet known if there will be crossover support. Reference
      /// https://en.wikipedia.org/wiki/FMA_instruction_set for more
      /// details. This means that we really can't know if a processor
      /// supports FMA3 or FMA4 without also knowing the processor brand.
      bool hasFMA(void) const;
      /// Has Fused Multiply Add 3 support.
      /// @note Don't rely on this too much as correct answers depend on the
      /// cpu maker and must be added to the implementation. That is, it is
      /// possible for a processor to support FMA3 while this function
      /// returns false. 
      bool hasFMA3(void) const;
      /// Has Fused Multiply Add 4 support.
      /// @note Don't rely on this too much as correct answers depend on the
      /// cpu maker and must be added to the implementation. That is, it is
      /// possible for a processor to support FMA3 while this function
      /// returns false.
      bool hasFMA4(void) const;

      /// Compare and Exchange on 16 bytes is supported.
      /// Means CMPXCHG16B instruction is implemented.
      bool hasCX16(void) const;

      /// Support for the xTPR Update Control over Task Priority messages.
      /// Task Priority messages can be optionally disabled through toggling
      /// bit 23 of the MISC_ENABLE model specific registers.
      /// @note Currently only intel is known to support this.
      bool hasETPRD(void) const;

      /// Has Performance Debug Capability MSR.
      /// For Intel this means MSR 0x345.
      /// @note AMD's CPUID manual makes no mention of this.
      bool hasPDCM(void) const;

      /// Process Context Identifiers
      /// Software may set CR4.PCIDE to 1.
      bool hasPCID(void) const;
      /// Direct Cache Access
      /// @note What this is used for is not well documented in the intel
      /// or AMD manuals.
      /// @todo Look up what this processor feature is for!
      bool hasDCA(void) const;
      /// SSE4.1 instructions are supported.
      bool hasSSE4_1(void) const;
      /// SSS4.2 instructions are supported.
      bool hasSSE4_2(void) const;
      
      /// Advanced Programmable Interrupt Controller version 2.
      /// See http://www.intel.com/Assets/pdf/manual/318148.pdf
      /// @note This is currently Intel only. 2011-11-20.
      bool hasX2APIC(void) const;

      /// Support for moving data after swapping bytes.
      /// Meant to assist with translating big endian data to little endian
      /// data. For example various internet protocols are done in big
      /// endian. Instruction, when availible, is MOVBE.
      /// @note This is currently only supported on Intel Atom
      /// processors. Newer Intel processors lack support for this
      /// instruction and no other CPU companies seem to have it as of
      /// 2011-11-20.
      bool hasMOVBE(void) const;
      
      /// Support for Population Count.
      /// The POPCNT instruction when supported computes the Hamming
      /// weight. This is the number of '1' bits there are in the operand.
      bool hasPOPCNT(void) const;

      /// One shot operation using a Time Stamp Counter.
      /// Local APIC timer is capable of one shot operation by using a Time
      /// Stamp Counter deadline value.
      /// @todo Figure out why someone would want to use this feature!
      bool hasTSCD(void) const;
      
      /// Advanced Encryption Standard instruction set support.
      /// Instructions added are:
      /// AESENC, AESENCLAST, AESDEC, AESDECLAST, AESKEYGENASSIST, AESIMC,
      /// PCLMULQDQ. 
      bool hasAES(void) const;
      
      /// Allows full or partial save of processor state components.
      /// Support for the following instructions:
      /// XSAVE, XRESTOR, XSETBV, XGETBV
      bool hasXSAVE(void) const;
      
      /// Allows the OS to turn on XSAVE.
      /// @todo What is this used for? Why is it useful/needed?
      bool hasOSXSAVE(void) const;
      
      /// Advanced Vector Extensions
      /// New YYM registers supported.
      bool hasAVX(void) const;
      
      /// Conversion of floating point vectors to half precision support.
      /// @note As of 2011-11 only AMD has support. Unknown if Intel will
      /// also pick it up or not.
      bool hasF16C(void) const;
      
      /// On chip Random Number Generator
      /// Indicates that cryptographically secure random numbers can be
      /// requested from the processor through the RDRAND instruction.
      /// @note Currently only some intel chips starting at Ivy Bridge have
      /// plans to support this instruction.  
      bool hasRDRAND(void) const;
      /// Normally zero unless we are running under a hypervisor.
      bool hasHypervisor(void) const;
      
      /// x87 Floating Point Unit supported.
      bool hasFPU(void) const;
      bool hasVME(void) const;
      bool hasDE(void) const;
      bool hasPSE(void) const;
      bool hasTSC(void) const;
      bool hasMSR(void) const;
      bool hasPAE(void) const;
      bool hasCX8(void) const;
      bool hasAPIC(void) const;
      bool hasSEP(void) const;
      bool hasMTRR(void) const;
      bool hasPGE(void) const;
      bool hasMCA(void) const;
      bool hasCMOV(void) const;
      bool hasPAT(void) const;
      bool hasPSE36(void) const;
      /// Processor Serial Number
      /// Disabled on more recent processors due to privacy issues.
      bool hasPSN(void) const;
      bool hasCLFL(void) const;
      bool hasDTES(void) const;
      bool hasACPI(void) const;
      bool hasMMX(void) const;
      bool hasFXSR(void) const;
      bool hasSSE(void) const;
      bool hasSSE2(void) const;
      bool hasSS(void) const;

      /// Does this processor support HyperThreading?
      bool hasHTT(void) const;
      bool hasTM1(void) const;
      bool hasIA64(void) const;
      bool hasPBE(void) const;

    private:
      bool _has_cpuid;
      
      MachineVendor _machine_vendor;
      ProcessorType _processor_type;
      u8 _family;   /// Combination of basefamily and extended family.
      u8 _model;    /// Combination of basemodel and extended model.
      u8 _stepping; /// Only lower 4 bits are ever used.
      
      u32 _features_ecx;
      u32 _features_edx;
      u8 _local_apic_id;
      u8 _logical_processor_count;
      u8 _clflush_size;
      u8 _brand_id;

      
    private:
      void fn0000_0001_eax(uint eax);
      void fn0000_0001_ebx(uint ebx);
      void fn0000_0002_eax(uint eax);
    };
  }
}
