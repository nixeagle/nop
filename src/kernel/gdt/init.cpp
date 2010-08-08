#include "gdt.h"
#include "descriptor.h"
#include "kernel/asm/lgdt.h"
namespace kernel {
  namespace gdt {

    BaseDescriptor<GdtEntry> init(void) {
      using kernel::gdt::AccessOptions;
      using kernel::gdt::FlagOptions;
      BaseDescriptor<GdtEntry> descs = BaseDescriptor<GdtEntry>(3);

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

      kernel::inlasm::lgdt(&descs);

      /** \retval GdtDescriptor is a fully loaded and initialized
          descriptor. The return result can be used for printing
          information about the gdt out for debugging.
      */
      return descs;
    }
  }
}
