#include "idt.h"
#include "kernel/gdt/descriptor.h"
#include "kernel/panic/kpanic.h"
namespace kernel {
  namespace idt {
    //    static IdtDescriptor idtd;
    //    static IdtGate idt_gates[256];
    //    static inline void loadIdt(const Idtd idtd) {
      //      asm volatile ("lidt %0" : : "m" (idtd));
    //    }

    using kernel::gdt::BaseDescriptor;
    BaseDescriptor<IdtEntry> init(uint16_t entry_count) {
      if (256 < entry_count) {
        KPANIC("entry_count larger then 256", "Positive overflow");
      } else {
        BaseDescriptor<IdtEntry> idt = BaseDescriptor<IdtEntry>(entry_count);
        return idt;
      }
    }
  }
}
