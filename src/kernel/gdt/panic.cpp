#include "kernel/gdt/gdt.h"
#include "kernel/panic/kpanic.h"
namespace kernel {
  namespace panic {
    void kpanic (kernel::gdt::GdtDescriptor* gdt_descriptor,
                 const char* message, const char* function,
                 int line, const char* file, const char* timestamp) {
      header(message, function, line, file, timestamp);
      gdt_descriptor->inspect(6);
      asm("hlt");
    }

    void kpanic (kernel::gdt::GdtEntry* gdt_entry,
                 const char* message, const char* function,
                 int line, const char* file, const char* timestamp) {
      header(message, function, line, file, timestamp);
      gdt_entry->inspect(6);
      asm("hlt");
    }
  }
}
