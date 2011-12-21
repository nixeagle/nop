#include "kernel.h"
//#include "kernel/text_mode/text_mode.h"
#include "kernel/gdt/gdt.h"
#include "kernel/idt/idt.h"
#include "kernel/gdt/descriptor.h"
#include "kernel/asm/out.h"
#include "kernel/asm/in.h"
#include "kernel/virtual-console/virtual-console.h"
#include "kernel/cpuid.h"
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

struct COMPort {
  // We have 8 different ones....
  u8 registers[8];
};

template<class T>
void show_type(const T&) {
  kernel::VirtualConsole::currentConsole()->put(__PRETTY_FUNCTION__);
}

static const u16* COM = reinterpret_cast<u16*>(0x400);
static void serializeString(const char* c_string) {
  char* ptr = const_cast<char*>(c_string);
  while (*ptr != 0) {
    while((kernel::inlasm::inb(COM[0]+5) & 0x20) == 0) {}
    kernel::inlasm::outb(COM[0]+0, *ptr);
    *ptr++;
  }
  
}
extern "C" void kmain(struct mb_header *header, unsigned int magic) {
  using kernel::gdt::BaseDescriptor;
  using kernel::gdt::GdtEntry;
  using kernel::idt::IdtEntry;
  using kernel::VirtualConsole;
  
  kernel::cpuid::CpuInfo cpuinfo;
  
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

  /*BaseDescriptor<GdtEntry> descs =*/ kernel::gdt::init();
  puts_allocated_memory();

  /*BaseDescriptor<IdtEntry> idt =*/ kernel::idt::init(256);

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
  puts_allocated_memory();

  //put_hex(kernel::cpuid::upperFunctionCode(),8,15);
  //put_hex(kernel::cpuid::cpuid(0x8000001E).eax(),8,15);
  //put_hex(static_cast<uint>(kernel::cpuid::hasCPUID()),8,15);
  put_hex(static_cast<uint>(cpuinfo.hasCPUID()),8,15);
  put_hex(static_cast<uint>(cpuinfo.hasSSE3()),8,25);
  //put_hex(static_cast<uint>(cpuinfo.vendor()),8,35);
  //put_hex(static_cast<uint>(cpuinfo.hasFPU()),9,15);
  //put_hex(static_cast<uint>(cpuinfo.hasCX16()),9,25);
  //put_hex(static_cast<uint>(cpuinfo.hasPDCM()),9,35);


  
  puts("COM1:", 10, 19);
  puts("COM2:", 10, 39);
  puts("COM3:", 11, 19);
  puts("COM4:", 11, 39);
  //#b11110000 00000000 11111111 01010011

  put_hex(COM[0],10,25);
  put_hex(COM[1],10,45);
  put_hex(COM[2],11,25);
  put_hex(COM[3],11,45);

  kernel::inlasm::outb(COM[0]+1, 0x00);
  kernel::inlasm::outb(COM[0]+3, 0x80);
  kernel::inlasm::outb(COM[0], 0x1);
  kernel::inlasm::outb(COM[0]+1, 0x00);
  kernel::inlasm::outb(COM[0]+3, 0x3);
  kernel::inlasm::outb(COM[0]+2, 0xc7);
  kernel::inlasm::outb(COM[0]+4, 0x0B);
  serializeString("Hello world!\n");
  while((kernel::inlasm::inb(COM[0]+5) & 0x20) == 0) {}
  // output letter 'S' for success.
  kernel::inlasm::outb(COM[0]+0, 83);


  //asm volatile("sti");
  // //  Enter experiments function, this returns void.
  //experiments::main();
  //busy_loop(0xfff);
  return;
}

