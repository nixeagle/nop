#include "kernel/gdt/gdt.h"
#include "kernel/panic/kpanic.h"
namespace kernel {
  namespace panic {
    __attribute__((noreturn)) inline static void halt () {
      asm volatile ("hlt");
      __builtin_unreachable();
    }
    void kpanic (kernel::gdt::GdtDescriptor* gdt_descriptor,
                 const char* message, const char* function,
                 int line, const char* file, const char* timestamp) {
      header(message, function, line, file, timestamp);
      gdt_descriptor->inspect(6);
      halt();
    }

    void kpanic (kernel::gdt::GdtEntry* gdt_entry,
                 const char* message, const char* function,
                 int line, const char* file, const char* timestamp) {
      header(message, function, line, file, timestamp);
      gdt_entry->inspect(6);
      halt();
    }
  }
}
