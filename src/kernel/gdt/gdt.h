#pragma once
#include "types.h"

namespace kernel {
  namespace gdt {

    class __attribute__((packed)) GdtEntry {
      uint16_t base;
      uint16_t limit;
      uint8_t base2;
      uint8_t access_byte;
      uint8_t flags_and_limit;
      uint8_t base3;
    };

    /** GDT description structure.

        This should be loaded by the LGDT assembly instruction by the
        kernel soon ater boot.
    */
    class __attribute__((packed)) GdtDescriptor {
      /** The size of the gdt entries.

          The size is one less then the actual table size as the table
          can hold 65536 entries but an \ref uint_16_t
          "unsigned 16 bit integer" only holds 65535 values. It is never
          valid for limit to be 0.

          @note osdev.org refers to this as the \e size
      */
      uint16_t limit;
      /** virtual address of the table itself.
          @note osdev.org refers to this as the \e offset.
      */
      GdtEntry* base;
    };
  }
}
