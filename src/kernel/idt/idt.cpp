#include "idt.h"
#include "kernel/gdt/descriptor.h"
namespace kernel {
  namespace idt {
    //    static IdtDescriptor idtd;
    //    static IdtGate idt_gates[256];
    //    static inline void loadIdt(const Idtd idtd) {
      //      asm volatile ("lidt %0" : : "m" (idtd));
    //    }

    using kernel::gdt::BaseDescriptor;
    BaseDescriptor<IdtEntry> init(uint16_t entry_count) {
      BaseDescriptor<IdtEntry> idt = BaseDescriptor<IdtEntry>(entry_count);
      return idt;
    }
  }
}
