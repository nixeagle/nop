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

    void IdtEntry::setEntry(void (*base)(), uint16_t selector, uint8_t flags) {
      setBase(reinterpret_cast<uint32_t>(base));
      setSelector(selector);
      setAccessByte(flags);
    }
    using kernel::gdt::BaseDescriptor;

    BaseDescriptor<IdtEntry> init(uint16_t entry_count) {
      using kernel::idt::AccessOptions;
      if (256 < entry_count) {
        KPANIC("entry_count larger then 256", "Positive overflow");
      } else {
        BaseDescriptor<IdtEntry> idt = BaseDescriptor<IdtEntry>(256);
        idt[0]->setEntry(&isr0, 0x08, Present | Ring0);
        idt[1]->setEntry(&isr1, 0x08, Present | Ring0);
        idt[2]->setEntry(&isr2, 0x08, Present | Ring0);
        idt[3]->setEntry(&isr3, 0x08, Present | Ring0);
        idt[4]->setEntry(&isr4, 0x08, Present | Ring0);
        idt[5]->setEntry(&isr5, 0x08, Present | Ring0);
        idt[6]->setEntry(&isr6, 0x08, Present | Ring0);
        idt[7]->setEntry(&isr7, 0x08, Present | Ring0);
        idt[8]->setEntry(&isr8, 0x08, Present | Ring0);
        idt[9]->setEntry(&isr9, 0x08, Present | Ring0);
        idt[10]->setEntry(&isr10, 0x08, Present | Ring0);
        idt[11]->setEntry(&isr11, 0x08, Present | Ring0);
        idt[12]->setEntry(&isr12, 0x08, Present | Ring0);
        idt[13]->setEntry(&isr13, 0x08, Present | Ring0);
        idt[14]->setEntry(&isr14, 0x08, Present | Ring0);
        idt[15]->setEntry(&isr15, 0x08, Present | Ring0);
        idt[16]->setEntry(&isr16, 0x08, Present | Ring0);
        idt[17]->setEntry(&isr17, 0x08, Present | Ring0);
        idt[18]->setEntry(&isr18, 0x08, Present | Ring0);
        idt[19]->setEntry(&isr19, 0x08, Present | Ring0);
        idt[20]->setEntry(&isr20, 0x08, Present | Ring0);
        idt[21]->setEntry(&isr21, 0x08, Present | Ring0);
        idt[22]->setEntry(&isr22, 0x08, Present | Ring0);
        idt[23]->setEntry(&isr23, 0x08, Present | Ring0);
        idt[24]->setEntry(&isr24, 0x08, Present | Ring0);
        idt[25]->setEntry(&isr25, 0x08, Present | Ring0);
        idt[26]->setEntry(&isr26, 0x08, Present | Ring0);
        idt[27]->setEntry(&isr27, 0x08, Present | Ring0);
        idt[28]->setEntry(&isr28, 0x08, Present | Ring0);
        idt[29]->setEntry(&isr29, 0x08, Present | Ring0);
        idt[30]->setEntry(&isr30, 0x08, Present | Ring0);
        idt[31]->setEntry(&isr31, 0x08, Present | Ring0);
        // idt.getBase()[3].setBase(reinterpret_cast<uint32_t>(&isr3));
        // idt.getBase()[3].setSelector(0x08);
        // idt.getBase()[3].setAccessByte(0x8E);

        asm("lidt %0" : : "m" (idt));
        //        kernel::inlasm::lidt(&idt);
        return idt;
      }
    }
  }
}
