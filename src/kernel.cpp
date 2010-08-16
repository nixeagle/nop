#include "kernel.h"
//#include "kernel/text_mode/text_mode.h"
#include "kernel/gdt/gdt.h"
#include "kernel/idt/idt.h"
#include "kernel/gdt/descriptor.h"
#include "kernel/asm/out.h"
#include "kernel/virtual-console/virtual-console.h"
#include "experiments/main.h"

using kernel::text_mode::put_hex;
using kernel::text_mode::puts;

void puts_allocated_memory() {
  puts("Allocated Mem - Kend", 0, 60);
  put_hex(kernel::memory::getAllocatedByteCount(), 1, 60);
  put_hex(reinterpret_cast<unsigned int>(&kernel_end), 1, 70);
}

void busy_loop(size_t limit) {
  // Busy loop.
  for(unsigned int i = 0;i < limit; i++) {
    for(volatile unsigned int i = 0; i < 10000000;) {
      i++;
    }
  }
  return;
}

extern "C" void kmain(struct mb_header *header, unsigned int magic) {
  using kernel::gdt::BaseDescriptor;
  using kernel::gdt::GdtEntry;
  using kernel::idt::IdtEntry;
  using kernel::VirtualConsole;
  using kernel::VirtualConsole;
  // Setup memory:
  kernel::text_mode::clear_screen();

  kernel::memory::init();

  if(0x2BADB002 != magic) {
    puts("ERROR: Bootloader magic does not match.", 15, 20);
    put_hex(magic,16,22);
  }
  //  void* foo = kernel::memory::kmalloc(10);
  puts("nop", 0, 0);
  //  puts_allocated_memory();

  BaseDescriptor<GdtEntry> descs = kernel::gdt::init();
  puts_allocated_memory();

  BaseDescriptor<IdtEntry> idt = kernel::idt::init(256);

  kernel::VirtualConsole vc[6];
  puts_allocated_memory();

  //  asm volatile ("xchg %bx, %bx");
  // asm volatile ("int $0x3");
  //
  asm volatile("sti");
  // timer tests.
  kernel::inlasm::outb(0x43, 0x36);
  kernel::inlasm::outb(0x40, 0xFF);
  kernel::inlasm::outb(0x40, 0xFF);


  //  Enter experiments function, this returns void.
  experiments::main();

  puts_allocated_memory();
  kernel::text_mode::putInteger(0xa, 16, 24,0);

  put_hex((size_t)kernel::memory::kmalloc(123),14, 0);
  put_hex((size_t)kernel::memory::kmalloc(123),14, 10);
  puts_allocated_memory();

  //  vc[0].setCurrent();
  vc[0].setCurrent();
  VirtualConsole::currentConsole()->clearBuffer();
  VirtualConsole::currentConsole()->put("test!");
  VirtualConsole::currentConsole()->put(" Hi?");
  VirtualConsole::currentConsole()->put(100);
  VirtualConsole::currentConsole()->updateOutputVideoRam();

  busy_loop(0x10);

  vc[1].setCurrent();
  VirtualConsole::currentConsole()->put(100);
  VirtualConsole::currentConsole()->putc('\n');
  VirtualConsole::currentConsole()->put("Hi!");
  VirtualConsole::currentConsole()->put(VirtualConsole::currentConsole());
  VirtualConsole::currentConsole()->updateOutputVideoRam();
  //  asm("int $3");

  busy_loop(0x1ff);
  return;
}

