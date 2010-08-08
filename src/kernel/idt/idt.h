#include "types.h"
#include "kernel/memory/memory.h"
#include "kernel/gdt/descriptor.h"

namespace kernel {
  namespace idt {

    enum AccessOptions {
      Ring0                 = 0b00000000,
      Ring1                 = 0b00100000,
      Ring2                 = 0b01000000,
      Ring3                 = 0b01100000,
      Present               = 0x80,
    };

    //    int init(uint8_t entry_count);
    class IdtEntry {
      uint16_t base_low;
      uint16_t selector; // Code segment selector. (GDT or LDT)
      uint8_t zero;
      uint8_t access_byte;
      uint16_t base_high;
      void setBase(size_t base) {
        base_low = base & 0xFFFF;
        base_high = static_cast<uint16_t>(base >> 16);
      }

      void setSelector(uint16_t selector) {
        this->selector = selector;
      }

      void setAccessByte(uint8_t access_byte) {
        this->access_byte = access_byte;
      }
    } __attribute__((packed));

    class IdtDescriptor {
      /// Length of idt array in \e bytes.
      /// 255 is the max permissable idt entries.
      uint16_t limit;
      IdtEntry* base;
    public:
      inline IdtEntry* getBase(void) {return base; }
      IdtDescriptor(uint8_t entry_count)
        /// @bug Possibly set to 0 if gcc misses out on function setting.
        : limit(setEntryCount(entry_count))
        , base(reinterpret_cast<IdtEntry*>(kernel::memory::kmalloc(limit + 1))) {}

      // /// @todo Check base's bounds against kernel boundery if possible.
      // int setBase(uint32_t base) {
      //   this->base = base;
      //   return 0; /// \suc0
      // }

      /** Set number of descriptor entries.

          \a entry_count is directly translated to a value suitable for
          \ref limit.

          \param[in] entry_count is an integer in the range  32 ... 255.

          \internal For x86 (32bit) descriptors are 8 bytes in size but
          this may vary for 64bit or 16 bit x86oid arches.
       */
      uint16_t setEntryCount(uint8_t entry_count) {
        limit = static_cast<uint16_t>(entry_count * sizeof(IdtEntry) - 1);
        return 0; /// \suc0
      }
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
