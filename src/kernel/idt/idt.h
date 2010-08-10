#pragma once
#include "types.h"
#include "kernel/memory/memory.h"
#include "kernel/gdt/descriptor.h"

namespace kernel {
  namespace idt {

    /** Enumerations for \e access_byte */
    enum AccessOptions {
      Ring0                 = 0b00000000,
      Ring1                 = 0b00100000,
      Ring2                 = 0b01000000,
      Ring3                 = 0b01100000,
      Present               = 0x80,
    };

    class __attribute__((packed)) IdtEntry {
      uint16_t base_low;
      uint16_t selector; /// Code segment selector. (GDT or LDT)
      uint8_t zero; /// Byte should be 0.
      uint8_t access_byte; /// Access flags, see \ref AccessOptions.
      uint16_t base_high;
    public:
      void setBase(size_t base) {
        base_low = base & 0xFFFF;
        base_high = static_cast<uint16_t>(base >> 16);
      }

      void setSelector(uint16_t selector) {
        this->selector = selector;
      }

      /** \param access_byte[in] should be set using \ref AccessOptions
          flags.
      */
      void setAccessByte(uint8_t access_byte) {
        this->access_byte = access_byte;
      }

      /** Show information about this entry */
      void inspect(size_t line_number);
    };

    /** Compute \e limit field for a gdt/idt/<etc> descriptor.

        \param limit[in] is number of entries in descriptor table. Valid
        range is [1 ... 255]. No descriptor table is valid with no
        entries, and no entries really does not make sense for any of
        these.
     */
    uint16_t computeLimit(uint8_t limit);
    /** Load interrupt descriptor table.

        \pre \a idtd is fully initialized with both \ref Idtd::limit
        "limit" and \ref Idtd::base "base" correctly set.

        \post Interrupt descriptor table loaded by the CPU.
     */

    kernel::gdt::BaseDescriptor<IdtEntry> init(uint16_t entry_count);
    //    static inline void loadIdt (const IdtDescriptor idtd);
  }
}
