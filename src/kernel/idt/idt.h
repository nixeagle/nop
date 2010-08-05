#include "types.h"

namespace kernel {
  namespace idt {

    int init(uint8_t entry_count);


    class IdtGate {
      uint16_t offset1;
      uint16_t selector; // Code segment selector. (GDT or LDT)
      uint8_t zero;
      uint8_t type_attributes;
      uint16_t offset2;


    };

    /** Default number of descriptors to put in interrupt table.

        This value is placed in the
        \ref Idtd "Interrupt Descriptor Table descriptor".

        \note 255 is the absolute maximum number of idt entries
        allowed.
     */
    const uint16_t kIDT_ENTRY_COUNT = 255;
    class Idtd {
      /// Length of idt array in \e bytes.
      uint16_t limit;
      uint32_t base;
    public:
      Idtd(void)
        : limit(static_cast<uint16_t>(kIDT_ENTRY_COUNT * sizeof(IdtGate)))
        , base(0x0) {}

      /// @todo Check base's bounds against kernel boundery if possible.
      int setBase(uint32_t base) {
        this->base = base;
        return 0; /// \suc0
      }

      /** Set number of descriptor entries.

          \a entry_count is directly translated to a value suitable for
          \ref limit.

          \param[in] entry_count is an integer in the range  32 ... 255.

          \internal For x86 (32bit) descriptors are 8 bytes in size but
          this may vary for 64bit or 16 bit x86oid arches.
       */
      int setEntryCount(uint8_t entry_count) {
        limit = (static_cast<uint16_t>(entry_count * sizeof(IdtGate)));
        return 0; /// \suc0
      }
    };

    /** Load interrupt descriptor table.

        \pre \a idtd is fully initialized with both \ref Idtd::limit
        "limit" and \ref Idtd::base "base" correctly set.

        \post Interrupt descriptor table loaded by the CPU.
     */
    static inline void loadIdt (const Idtd idtd);
  }
}
