#pragma once
#include "types.h"
#include "kernel/memory/memory.h"
#include "kernel/text_mode/text_mode.h"

namespace kernel {
  namespace gdt {

    /** Template for descriptor entry tables.

        This will be used for gdt, idt, and possibly others that follow
        the pattern.
     */
    template <class T>
    class __attribute__((packed)) BaseDescriptor {
    private:
      /** Length of descriptor table in bytes

          255 is the max permissable descriptor entries.
      */
      uint16_t limit;
      /// Pointer to the actual table of entries.
      T* base;

    protected:
      /** Set number of descriptor entries.

          \a entry_count is directly translated to a value suitable for
          \ref limit.

          \param[in] entry_count is an integer in the range  32 ... 255.

          \internal For x86 (32bit) descriptors are 8 bytes in size but
          this may vary for 64bit or 16 bit x86oid arches.
      */
      uint16_t setEntryCount(uint8_t entry_count) {
        limit = static_cast<uint16_t>(entry_count * sizeof(T) - 1);
        return 0; /// \suc0
      }


    public:
      inline T* getBase(void) { return base; }
      inline uint16_t getLimit(void) { return limit; }

      /// Number of entries in descriptor table.
      inline uint16_t entryCount(void) {
        return static_cast<uint16_t>(getLimit() + 1) / sizeof(T);
      }

      /** Construct and allocate a base descriptor.

          \param entry_count[in] Number of entries to create in the
          table. This \e must be 256 or less and can \e never be 0.
       */
      BaseDescriptor(uint16_t entry_count)
        : limit(static_cast<uint16_t>((entry_count * sizeof(T)) - 1))
        , base(reinterpret_cast<T*>(kernel::memory::kmalloc(limit + 1))) {}


      /** Print out the descriptor table and some entries to conosole.

          \param line[in] is the line number to start printing inspect
          information out to.
       */
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
