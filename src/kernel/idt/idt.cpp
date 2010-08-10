#include "idt.h"
#include "kernel/gdt/descriptor.h"
#include "kernel/panic/kpanic.h"
#include "kernel/asm/lidt.h"
#include "kernel/isr/isr.h"

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
        BaseDescriptor<IdtEntry> idt = BaseDescriptor<IdtEntry>(256);

        //        kernel::text_mode::put_hex((size_t)idt, 2, 40);
        idt.getBase()[3].setBase(reinterpret_cast<uint32_t>(&isr3));
        idt.getBase()[3].setSelector(0x08);
        idt.getBase()[3].setAccessByte(0x8E);

        asm("lidt %0" : : "m" (idt));
        //        kernel::inlasm::lidt(&idt);
        return idt;
      }
    }
  }
}
