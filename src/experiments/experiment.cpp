#include "experiment.h"
namespace experiments {
  void Block::free() {
    allocatedp = false;
  }

  void* Block::alloc(size_t start, uint16_t size) {
    this->start = start;
    this->size = size;
    allocatedp = true;
    kernel::text_mode::put_hex(start, 7, 60);
    kernel::text_mode::put_hex(this->start, 7, 70);
    /// \retval void* is a pointer to newly allocated memory block.
    return reinterpret_cast<void*>(start);
  }
  bool Block::freeIf(size_t memory_address) {
    kernel::text_mode::put_hex(memory_address, 7, 3);
    kernel::text_mode::put_hex(start, 7, 13);
    if(memory_address == start) {
      free();
      return true;
    } else {
      return false;
    }
  }

  void* Block::allocIf(uint16_t size) {
    if (!allocatedp && start != 0x0 && this->size >= size) {
      /// \retval "any non null pointer" is a pointer to newly alloated
      /// memory.
      return alloc(this->start, size);
    } else {
      /// \retval (void*)0x0 Pointer to absolute start of physical memory
      /// means nothing was allocated.
      return reinterpret_cast<void*>(0x0);
    }
  }

  bool Block::brandNewP() {
    if(!allocatedp && start == 0x0 && size == 0xBEEF) {
      return true;
    } else {
      return false;
    }
  }

  void Heap::free(size_t memory_address) {
    kernel::text_mode::puts(__PRETTY_FUNCTION__, 11, 0);
    kernel::text_mode::put_hex(memory_address, 12, 3);
    // Look for memory_address in the lookup array.
    for(int i = 0; i < heap_size; i++) {
      if(heap[i].freeIf(memory_address)) {
        return;
      }
      kernel::text_mode::put_hex(i, 12, 13);
    }
    asm("int $6;");             // Something went wrong.
  }

  Block* Heap::findBrandNewBlock(void) {
    for(int i = 0; i < heap_size; i++) {
      kernel::text_mode::puts(__PRETTY_FUNCTION__, 15, 0);
      kernel::text_mode::put_hex(i, 16, 3);
      if(heap[i].brandNewP()) {

        return &heap[i];
      }
    }
    // oh crap! out of suitable ram.
    asm("int $4");
  }

  void* Heap::malloc(uint16_t size) {
    // First try to allocate in an existing block that is not used.
    for(int i = 0; i < heap_size; i++) {
      void* new_memory = heap[i].allocIf(size);
      if(reinterpret_cast<void*>(0x0) != new_memory) {
        kernel::text_mode::put_hex(reinterpret_cast<size_t>(new_memory), 5, 3);
        return new_memory;
      }
    }
    void* new_memory = findBrandNewBlock()->alloc(heap_end, size);
    heap_end += size;
    kernel::text_mode::put_hex(reinterpret_cast<size_t>(new_memory), 5, 3);
    return new_memory;
  }
}
