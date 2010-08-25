#include "queue.h"

namespace nib {
  // template<typename T>
  // Queue<T>::Queue() : start(*new Node<T>(0,0)), end(start)
  //                    {}

  // template<typename T>
  // Queue<T>::~Queue() {
  //   delete start;
  // }

  // template<typename T>
  // void Queue<T>::push(T& element) {
  //   if(start->emptyp()) {
  //     start.setElement(element);
  //     start.setNextNode(&start);
  //   } else {
  //     end.setNextNode(new Node<T>(element));
  //     end = end.getNextNode(); // end moves up.
  //   }
  // }
  // template<typename T>
  // size_t Queue<T>::size() {
  //   if(start.emptyp()) {
  //     return 0;
  //   } else {
  //     Node<T>& node = start;
  //     for(int i = 1; !node.tailp(); i++) {
  //       node = node.getNextNode();
  //     }
  //   }
  // }
}
