#include "isr.h"
#include "kernel/text_mode/text_mode.h"
extern "C" void isrHandler(volatile kernel::idt::Registers reg) {
  kernel::text_mode::puts("SOMEBODY DID INTERRUPT NUMBER: ", 2, 0);
  kernel::text_mode::put_hex(reg.int_no, 2, 32);
  if(0x03 == reg.int_no) {
    //    asm("cli; hlt");
  }
}
