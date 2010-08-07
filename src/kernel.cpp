#include "kernel.h"
#include "kernel/idt/idt.h"
#include "kernel/text_mode/text_mode.h"
#include "kernel/asm/lgdt.h"
#include "kernel/gdt/gdt.h"
using kernel::text_mode::put_hex;
using kernel::text_mode::puts;

void puts_allocated_memory() {
  puts("Allocated Mem - Kend", 0, 60);
  put_hex(kernel::memory::getAllocatedByteCount(), 1, 60);
  put_hex(reinterpret_cast<unsigned int>(&kernel_end), 1, 70);
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



  GdtDescriptor descs = GdtDescriptor(1);
  puts_allocated_memory();
  asm("lgdt %0": : "m" (descs));

  puts("------ GDT descriptor data ------", 1, 0);
  puts("Base:", 2, 0);
  put_hex(reinterpret_cast<unsigned int>(descs.getBase()), 2, 6);
  puts("Limit:", 2, 15);
  put_hex(descs.getLimit(), 2, 22);


  //  kernel::inlasm::lgdt((kernel::gdt::GdtDescriptor*)1);
  for(unsigned int i = 0;; i++) {
    put_hex(i, 5,30);
    for(unsigned int i = 0; i < 10000000;) {
      i++;
    }
  }
  return;
}
