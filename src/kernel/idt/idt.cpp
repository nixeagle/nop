#include "idt.h"

namespace kernel {
  namespace idt {
    static Idtd idtd;
    static IdtGate idt_gates[256];

    static inline void loadIdt(const Idtd idtd) {
      //      asm volatile ("lidt %0" : : "m" (idtd));
    }

    int init(uint8_t entry_count) {
      idtd.setBase(reinterpret_cast<uint32_t>(idt_gates));
      idtd.setEntryCount(entry_count);
      loadIdt(idtd);
      return 0; /// \suc0
    }
  }
}
