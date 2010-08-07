#include "kernel.h"
#include "kernel/text_mode/text_mode.h"
#include "kernel/gdt/gdt.h"
#include "kernel/panic/kpanic.h"

using kernel::text_mode::put_hex;
using kernel::text_mode::puts;

void puts_allocated_memory() {
  puts("Allocated Mem - Kend", 0, 60);
  put_hex(kernel::memory::getAllocatedByteCount(), 1, 60);
  put_hex(reinterpret_cast<unsigned int>(&kernel_end), 1, 70);
}

void busy_loop() {
  // Busy loop.
  for(unsigned int i = 0;i < 0x1ff; i++) {
    put_hex(i, 10,30);
    for(volatile unsigned int i = 0; i < 10000000;) {
      i++;
    }
  }
  return;
}

extern "C" void kmain(struct mb_header *header, unsigned int magic) {
  using kernel::gdt::GdtDescriptor;
  // Setup memory:
  kernel::text_mode::clear_screen();

  if(0x2BADB002 != magic) {
    puts(p("ERROR: Bootloader magic does not match."), 15, 20);
    put_hex(magic,16,22);
  }
  //  kernel::idt::init(255);

  puts("nop", 0, 0);
  puts_allocated_memory();

  GdtDescriptor descs = kernel::gdt::init();

  descs.inspect();
  puts_allocated_memory();

  kernel::panic::kpanic("crap!", "ooh!", __PRETTY_FUNCTION__, __LINE__, __FILE__, __TIMESTAMP__);
  busy_loop();
  return;
}
