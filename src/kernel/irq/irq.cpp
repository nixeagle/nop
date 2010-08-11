#include "irq.h"
#include "kernel/text_mode/text_mode.h"
#include "kernel/asm/out.h"

extern "C" void irqHandler(volatile kernel::idt::Registers reg) {
  if(reg.int_no >= 40) {
    kernel::inlasm::outb(0xA0, 0x20);
  }
  kernel::inlasm::outb(0x20, 0x20);
  kernel::text_mode::puts("IRQ TRIGGERED", 0, 40);
  kernel::text_mode::put_hex(reg.int_no, 1, 40);
}
