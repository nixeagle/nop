#include "idt.h"
#include "kernel/gdt/descriptor.h"
#include "kernel/panic/kpanic.h"
//#include "kernel/asm/lidt.h"
#include "kernel/isr/isr.h"
#include "kernel/asm/out.h"
#include "kernel/irq/irq.h"
namespace kernel {
  namespace idt {
    //    static IdtDescriptor idtd;
    //    static IdtGate idt_gates[256];
    //    static inline void loadIdt(const Idtd idtd) {
      //      asm volatile ("lidt %0" : : "m" (idtd));
    //    }
    using kernel::gdt::BaseDescriptor;

    static BaseDescriptor<IdtEntry>* remapIrqTable(BaseDescriptor<IdtEntry>* idt) {
      /// @todo figure out why using [] overload won't work with a pointer
      /// here but works with the no pointer stuff in \ref init

        idt->getBase()[32].setEntry(&irq0, 0x08, Present | Ring0);
        idt->getBase()[33].setEntry(&irq1, 0x08, Present | Ring0);
        idt->getBase()[34].setEntry(&irq2, 0x08, Present | Ring0);
        idt->getBase()[35].setEntry(&irq3, 0x08, Present | Ring0);
        idt->getBase()[36].setEntry(&irq4, 0x08, Present | Ring0);
        idt->getBase()[37].setEntry(&irq5, 0x08, Present | Ring0);
        idt->getBase()[38].setEntry(&irq6, 0x08, Present | Ring0);
        idt->getBase()[39].setEntry(&irq7, 0x08, Present | Ring0);
        idt->getBase()[40].setEntry(&irq8, 0x08, Present | Ring0);
        idt->getBase()[41].setEntry(&irq9, 0x08, Present | Ring0);
        idt->getBase()[42].setEntry(&irq10, 0x08, Present | Ring0);
        idt->getBase()[43].setEntry(&irq11, 0x08, Present | Ring0);
        idt->getBase()[44].setEntry(&irq12, 0x08, Present | Ring0);
        idt->getBase()[45].setEntry(&irq13, 0x08, Present | Ring0);
        idt->getBase()[46].setEntry(&irq14, 0x08, Present | Ring0);
        idt->getBase()[47].setEntry(&irq15, 0x08, Present | Ring0);

      return idt;
    }

    /** Remap the PIC */
    static void remapPic(void) {
      using kernel::inlasm::outb;
      outb(0x20, 0x11);
      outb(0xA0, 0x11);
      outb(0x21, 0x20);
      outb(0xA1, 0x28);
      outb(0x21, 0x04);
      outb(0xA1, 0x02);
      outb(0x21, 0x01);
      outb(0xA1, 0x01);
      outb(0x21, 0x0);
      outb(0xA1, 0x0);
    }
    void IdtEntry::setEntry(void (*base)(), uint16_t selector, uint8_t flags) {
      setBase(reinterpret_cast<uint32_t>(base));
      setSelector(selector);
      setAccessByte(flags);
    }


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
        remapPic();
        remapIrqTable(&idt);
        //        kernel::inlasm::lidt(&idt);
        return idt;
      }
    }
  }
}
