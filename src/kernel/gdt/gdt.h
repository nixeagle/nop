#pragma once
#include "types.h"
#include "kernel/memory/memory.h"

namespace kernel {
  namespace gdt {

    /** Options for \ref GdtEntry#access_byte.

     All options are independent except for the \a Ring0 to \a Ring3.
    */
    enum AccessOptions {
      Read__Write           = 0x02,
      Direction__Conforming = 0x04,
      Executable            = 0x08,
      Ring0                 = 0b00000000,
      Ring1                 = 0b00100000,
      Ring2                 = 0b01000000,
      Ring3                 = 0b01100000,
      Present               = 0x80,
    };

    /** Options for \ref flags_and_limit "extra flags". */
    enum FlagOptions {
      Granularity = 0x80,
      Size = 0x40,
    };

    /** Entry in the GDT

        This is a very strange structure due to how x86 was developed. The
        structure is 8 bytes. \ref limit and \ref base are both split out
        into parts.

        \e base is a 32 bit value holding the linear address where the
        segment begins.

        \e limit is a 20 bit value that indicates how much of the
        addressable space is covered by this segment. The size of each
        unit depends on what the value of the maximum addressable unit
        is. This is either 1 byte or 4KiB.
     */
    class __attribute__((packed)) GdtEntry {
      uint16_t limit;
      uint16_t base;
      uint8_t base2;

      /** Indicates permissons information for this entry.

          Bit fields are:
          \li [bit 7] \b Present - Must be 1 for all valid selectors
          \li [bit 6 ... 5] \b Privilege - Contains the ring level. \b 0
          is the highest and 3 is the lowest.
          \li [bit 4] is always set to \b 1.
          \li [bit 3] \b Executable - \b 1 if code in this segment is
          executable otherwise \b 0.
          \li [bit 2] \b Direction \b bit or \b Conforming \b Bit - On \e
          data \e selectors it indicates the direction the segment "grows"
          with \b 0 meaning the segment grows up and \b 1 meaning the
          segment grows down. On \e code \e selectors \b 1 indicates that
          code can be executed by an equal or lower priviledge level while
          \b 0 means can code can only be executed from the ring set in \e
          Priviledge.
          \li [bit 1] \b Read/Write - On \e code \e selectors \b 1 means
          the segment can be read from. For \e data \e segment \b 1 means
          the segment can be written to.
          \li [bit 0] \b Accessed - Should be set to \b 0, the CPU will
          set to \b 1 when the segment is accessed.
       */
      uint8_t access_byte;
      /** Extra flags

          \li [bit 7] \b Granularity - When \b 0 the \e limit is in 1 byte
          blocks otherwise when \b 1 the \e limit is in 4KiB blocks.
          \li [bit 6] \b Size - When \b 0 selector is in 16 bit protected
          mode, otherwise when \b 1 selector is in 32 bit protected mode.
          \li [bit 5 ... 4] are both 0.
          \li [bit 3 ... 0] are part of the \e limit.
       */
      uint8_t flags_and_limit;
      uint8_t base3;

    public:
      void setLimit(size_t limit);
      void setBase(size_t base);
      /// @note Osdev refers to this as the "type".
      int setAccessByte(uint8_t type) {
        access_byte = type | 0b00010000;
        return 0; /// \suc0
      }
      void setFlags(uint8_t flags);

      /// "inspect" by printing information about this entry to console.
      int inspect(uint8_t line);
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

    public:
      /** \retval base is a pointer to \ref base

       The result of this can be modified as if it were the real
       value. Changing what the pointer points at is a bad idea.
      */
      inline GdtEntry* getBase(void) { return base; }
      /// \retval the \ref limit
      inline uint16_t getLimit(void) { return limit; }

      inline uint16_t getEntryCount(void) {
        return static_cast<uint16_t>(getLimit() + 1) / sizeof(GdtEntry);
      }
      GdtDescriptor(uint16_t entry_count)
        : limit(static_cast<uint16_t>((entry_count * sizeof(GdtEntry)) - 1))
        , base(reinterpret_cast<GdtEntry*>(kernel::memory::kmalloc(limit + 1))) {}

      int inspect(uint8_t line);
    };

    /** Initialize GDT in a flat memory model. */
    GdtDescriptor init(void);
  }
}

namespace kernel {
  namespace panic {
    void kpanic (kernel::gdt::GdtDescriptor* gdt_descriptor,
                 const char* message, const char* function,
                 int line, const char* file, const char* timestamp)
      __attribute__ ((noreturn));

    void kpanic (kernel::gdt::GdtEntry* gdt_entry,
                 const char* message, const char* function,
                 int line, const char* file, const char* timestamp)
      __attribute__ ((noreturn));
  }
}
