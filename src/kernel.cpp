#include "kernel.h"
#include "kcommon.h"
#include "icxxabi.h"
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


template<class T>
void show_type(const T&) {
  kernel::VirtualConsole::currentConsole()->put(__PRETTY_FUNCTION__);
}
extern "C" size_t start_tests, end_tests;

  T(it, kernel::global::testout << "hi????"; return false;);

extern "C" void kmain(struct mb_header *header, unsigned int magic) {
  using kernel::gdt::BaseDescriptor;
  using kernel::gdt::GdtEntry;
  using kernel::idt::IdtEntry;
  using kernel::VirtualConsole;
  using kernel::VirtualConsole;
  // Setup memory:
  kernel::text_mode::clear_screen();

  kernel::text_mode::clear_screen();

  if(0x2BADB002 != magic) {
    puts("ERROR: Bootloader magic does not match.", 15, 20);
    put_hex(magic,16,22);
  }

  /// initialize consoles right away so we have a place to print test data
  /// out to.
  kernel::global::virtual_consoles = new VirtualConsole[6];
  kernel::global::virtual_consoles[0].setCurrent();
  kernel::global::testout = kernel::global::virtual_consoles[5];

  BaseDescriptor<GdtEntry> descs = kernel::gdt::init();
  puts_allocated_memory();

  BaseDescriptor<IdtEntry> idt = kernel::idt::init(256);

  //  kernel::VirtualConsole vc[6];

  puts_allocated_memory();



  // //  asm volatile ("xchg %bx, %bx");
  // // asm volatile ("int $0x3");
  // //

  // timer tests.
  kernel::inlasm::outb(0x43, 0x36);
  kernel::inlasm::outb(0x40, 0xFF);
  kernel::inlasm::outb(0x40, 0xFF);

  puts_allocated_memory();
  kernel::text_mode::putInteger(0xa, 16, 24,0);

  put_hex((size_t)kernel::memory::kmalloc(123),14, 0);
  put_hex((size_t)kernel::memory::kmalloc(123),14, 10);
  puts_allocated_memory();

  //  vc[0].setCurrent();
  kernel::global::virtual_consoles = new VirtualConsole[6];
  kernel::global::virtual_consoles[0].setCurrent();
  kernel::global::testout = kernel::global::virtual_consoles[5];
  puts_allocated_memory();

  asm volatile("sti");
  put_hex((size_t)&start_tests, 0, 0);
  put_hex((size_t)&end_tests, 0, 10);
  tests::runTests();



  experiments::main();
  busy_loop(0xfff);
  return;
}

