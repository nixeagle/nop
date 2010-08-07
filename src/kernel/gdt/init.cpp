#include "gdt.h"

namespace kernel {
  namespace gdt {

    GdtDescriptor init(void) {
      using kernel::gdt::AccessOptions;
      using kernel::gdt::FlagOptions;
      GdtDescriptor descs = GdtDescriptor(3);

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

      asm("lgdt %0": : "m" (descs));
      return descs;
    }
  }
}
