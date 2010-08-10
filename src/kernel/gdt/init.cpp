
#include "gdt.h"
#include "descriptor.h"
//#include "kernel/asm/lgdt.h"

namespace kernel {
  namespace gdt {
    BaseDescriptor<GdtEntry> descs = BaseDescriptor<GdtEntry>(3);
    BaseDescriptor<GdtEntry> init(void) {
      using kernel::gdt::AccessOptions;
      using kernel::gdt::FlagOptions;


      descs.getBase()[1].setLimit(0xfffff);
      descs.getBase()[1].setAccessByte(AccessOptions::Executable
                                       | AccessOptions::Ring0
                                       | AccessOptions::Present
                                       | AccessOptions::Read__Write);
      descs.getBase()[1].setFlags(FlagOptions::Granularity
                                  | FlagOptions::Size);

      descs.getBase()[2].setLimit(0xfffff);
      descs.getBase()[2].setAccessByte(
                                       AccessOptions::Ring0
                                       | AccessOptions::Present
                                       | AccessOptions::Read__Write);
      descs.getBase()[2].setFlags(FlagOptions::Granularity
                                  | FlagOptions::Size);

      ///@todo get kernel::inlasm inline assembly namespace up and running
      ///again
      //kernel::inlasm::lgdt(&descs);
      asm volatile ("lgdt %0;"
                    ".intel_syntax noprefix ;"
                    "jmp 0x8:.flush ;"
                    ".flush: ;"
                    "mov ax, 0x10 ;"
                    "mov ds, ax ;"
                    "mov es, ax ;"
                    "mov fs, ax ;"
                    "mov gs, ax ;"
                    "mov ss, ax ;"
                    ".att_syntax prefix ;"
                    : : "m" (descs)
                    : "ax");
      /** \retval GdtDescriptor is a fully loaded and initialized
          descriptor. The return result can be used for printing
          information about the gdt out for debugging.
      */
      return descs;
    }
  }
}
