/** Various isr handling routines

    These are all critical to handling x86 interrupts.
 */
#pragma once
#include "types.h"
namespace kernel {
  namespace idt {
    /** Defines what the stack looks like after an isr ran. */
    class Registers {
    public:
      uint32_t gs, fs, es, ds;      /* pushed the segs last */
      uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
      uint32_t err_code, int_no;    /* our 'push byte #' and ecodes do this */
      uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
    };
  }
}


extern "C" void isr0 ();
extern "C" void isr1 ();
extern "C" void isr2 ();
extern "C" void isr3 ();
extern "C" void isr4 ();
extern "C" void isr5 ();
extern "C" void isr6 ();
extern "C" void isr7 ();
extern "C" void isr8 ();
extern "C" void isr9 ();
extern "C" void isr10 ();
extern "C" void isr11 ();
extern "C" void isr12 ();
extern "C" void isr13 ();
extern "C" void isr14 ();
extern "C" void isr15 ();
extern "C" void isr16 ();
extern "C" void isr17 ();
extern "C" void isr18 ();
extern "C" void isr19 ();
extern "C" void isr20 ();
extern "C" void isr21 ();
extern "C" void isr22 ();
extern "C" void isr23 ();
extern "C" void isr24 ();
extern "C" void isr25 ();
extern "C" void isr26 ();
extern "C" void isr27 ();
extern "C" void isr28 ();
extern "C" void isr29 ();
extern "C" void isr30 ();
extern "C" void isr31 ();

extern "C" void __attribute__((noinline, optimize ("-O0"))) isrHandler(volatile kernel::idt::Registers reg);
