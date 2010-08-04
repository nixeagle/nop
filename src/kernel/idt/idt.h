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

    const uint16_t kIDT_ENTRY_COUNT = 255;
    class __attribute__ ((__packed__)) Idtd {
      uint16_t limit;
      uint32_t base;
    public:
      Idtd(void)
        : limit(static_cast<uint16_t>(kIDT_ENTRY_COUNT * sizeof(IdtGate)))
        , base(0x0) {}

      // TODO: Check base's bounds against kernel boundery if possible.
      int setBase(uint32_t base) { this->base = base; return 0; }
      int setEntryCount(uint8_t entry_count) {
        limit = (static_cast<uint16_t>(entry_count * sizeof(IdtGate)));
        return 0;
      }
      //      Idtd(uint32_t base_address, unsigned char entry_count)
      //        : limit(static_cast<uint16_t>(entry_count * sizeof(IdtGate)))
      //        , base(base_address) {}
    };

    static inline void loadIdt (const Idtd idtd);
  }
}
