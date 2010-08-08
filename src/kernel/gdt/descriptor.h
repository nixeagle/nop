#pragma once
#include "types.h"
#include "kernel/memory/memory.h"
#include "kernel/text_mode/text_mode.h"

namespace kernel {
  namespace gdt {
    /** Generic descriptor template */
    template <class T, class R>
    class mypair {
      T value;
      R value2;
    };

    // class BaseEntry {
    //   uint32_t a;
    //   uint32_t b;
    // };

    template <class T>
    class BaseDescriptor {
    private:
      uint16_t limit;
      T* base;
    public:
      inline T* getBase(void) { return base; }
      inline uint16_t getLimit(void) {return limit; }
      inline uint16_t entryCount(void) {
        return static_cast<uint16_t>(getLimit() + 1) / sizeof(T);
      }
      BaseDescriptor(uint16_t entry_count)
        : limit(static_cast<uint16_t>((entry_count * sizeof(T)) - 1))
        , base(reinterpret_cast<T*>(kernel::memory::kmalloc(limit + 1))) {}
      //      BaseEntry* base;

      void inspect(uint8_t line) {
        using kernel::text_mode::puts;
        using kernel::text_mode::put_hex;
        puts("------ Descriptor data ------", line, 0);
        puts("Base:", line+1, 0);
        put_hex(reinterpret_cast<unsigned int>(getBase()), line+1, 6);
        puts("Limit:", line+1, 15);
        put_hex(getLimit(), line+1, 22);
        puts("Entry Count:", line+1, 32);
        put_hex(entryCount(), line+1, 45);

        /// @todo need constant \ref kernel::text_mode::LINES in header
        /// file, currently we just hardcode the constant.
        for(int i = 0; i < entryCount() && i < 25 - line - 4; i++) {
          getBase()[i].inspect(static_cast<uint8_t>(i+3+line));
        }

      }
    };
  }
}
