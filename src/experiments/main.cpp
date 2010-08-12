#include "main.h"
#include "experiment.h"

namespace experiments {

  void main() {
    Heap heap = Heap();
    heap.inspect();
    size_t address = reinterpret_cast<size_t>(heap.malloc(12));
    heap.free(address);
    address = reinterpret_cast<size_t>(heap.malloc(12));
    size_t address2 = reinterpret_cast<size_t>(heap.malloc(12));
    heap.free(address);
    address = reinterpret_cast<size_t>(heap.malloc(20));
    heap.getBlock(0).inspect(17);
    heap.getBlock(1).inspect(18);
    heap.getBlock(2).inspect(19);
    heap.getBlock(3).inspect(20);
    //    size_t address3 = reinterpret_cast<size_t>(heap.malloc(12));
    heap.inspect();
  }
}
