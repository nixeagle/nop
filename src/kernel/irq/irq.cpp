#include "irq.h"
#include "kernel/text_mode/text_mode.h"
#include "kernel/asm/out.h"
static uint32_t tick = 0;
extern "C" void irqHandler(volatile kernel::idt::Registers reg) {

  kernel::text_mode::puts("IRQ TRIGGERED", 0, 40);
  kernel::text_mode::put_hex(reg.int_no, 1, 40);

  asm("cli");

  kernel::text_mode::put_hex(tick, 2, 41);
  if(reg.int_no >= 40) {
    kernel::inlasm::outb(0xA0, 0x20);
  }
  kernel::inlasm::outb(0x20, 0x20);
  tick++;

}
