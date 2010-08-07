#include "kernel.h"
#include "kernel/idt/idt.h"
#include "kernel/text_mode/text_mode.h"
#include "kernel/asm/lgdt.h"
#include "kernel/gdt/gdt.h"

extern "C" void kmain(struct mb_header *header, unsigned int magic) {
  // Setup memory:
  kernel::text_mode::clear_screen();

  if(0x2BADB002 != magic) {
    kernel::text_mode::puts(p("ERROR: Bootloader magic does not match."), 15, 20);
    kernel::text_mode::put_hex(magic,16,22);
  }

  kernel::idt::init(255);
  kernel::text_mode::put_hex(0x12300feb,6,3);
  kernel::text_mode::put_hex((unsigned int)&kernel_end,8,9);
  kernel::text_mode::puts(p("nop"), 0, 0);


  kernel::text_mode::put_hex(kernel::memory::getAllocatedByteCount(),10,9);
  kernel::text_mode::put_hex((unsigned int)kernel_end,11,9);

  kernel::text_mode::put_hex(sizeof(long int), 0, 30);
  // for(unsigned short int i = 0, j = 1; i < 80; i++, j++) {
  //   kernel::text_mode::put_char('H', 0, ++i);
  //   kernel::text_mode::put_char('H', 0, ++j);
  // }

  kernel::text_mode::put_hex(reinterpret_cast<unsigned int>(&kernel_end), 2, 32);
  kernel::gdt::GdtDescriptor descs =
    kernel::gdt::GdtDescriptor(1);

  kernel::text_mode::put_hex(reinterpret_cast<unsigned int>(descs.getBase())
                             ,3 , 40);

  kernel::text_mode::put_hex(reinterpret_cast<unsigned int>(&kernel_end), 2, 42);
  kernel::text_mode::put_hex(kernel::memory::getAllocatedByteCount(), 3, 60);

  kernel::gdt::GdtDescriptor descs2 =
    kernel::gdt::GdtDescriptor(3);

  kernel::text_mode::put_hex(reinterpret_cast<unsigned int>(descs2.getBase())
                             ,4 , 40);

  kernel::text_mode::put_hex(descs.getLimit(), 3, 50);

  //  kernel::inlasm::lgdt((kernel::gdt::GdtDescriptor*)1);
  for(unsigned int i = 0;; i++) {
    kernel::text_mode::put_hex(i, 5,30);
    for(unsigned int i = 0; i < 10000000;) {
      i++;
    }
  }
  return;
}
