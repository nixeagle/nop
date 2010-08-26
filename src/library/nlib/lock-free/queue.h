#pragma once
#include "kcommon.h"
#include "types.h"
/** Eventually implement lockfree algorithm.
    http://www.research.ibm.com/people/m/michael/podc-1996.pdf
*/

namespace  nib {
  using kernel::global::kout;
  template<typename T>
  class Queue {
    template<typename R>
    class Node; //forward declaration
  public:
    Queue(void) : start(new Node<T>(0,0)), end(start) { }

    /// Delete the entire queue.
    /// @todo what happens when the queue is not empty?
    ~Queue() {
      delete start;
    }
    T& front(void) {
      return start->getElement();
    }
    const T& front(void) const {
      return start->getElement();
    }
    /// Push \a element into the queue.
    void push(T& element) {
      if(end->emptyp()) {
        end->setElement(element);
        end->setNextNode(*end);
        kout << "\n" << "node: " << end << " Next node: " << &end->getNextNode();
      } else {
        Node<T>& next = *new Node<T>(element);
        kout << "\n" << "node: " << end << " Next node: " << &next << "\n";
        //end = &(end->getNextNode());
        end->setNextNode(next);
        end = &next;
        kout << "\n" << end;
        //kout << "\n" << "Next node?: " << &(end->getNextNode()) << "\n";
        ; // end moves up.
      }
    }
    void push(T&& element) {
      if(end->emptyp()) {
        end->setElement(element);
        end->setNextNode(*end);
        kout << "\n" << "node: " << end << " Next node: " << &end->getNextNode();
      } else {
        Node<T>& next = *new Node<T>(element);
        kout << "\n" << "node: " << end << " Next node: " << &next << "\n";
        //end = &(end->getNextNode());
        end->setNextNode(next);
        end = &next;
        kout << "\n" << end;
        //kout << "\n" << "Next node?: " << &(end->getNextNode()) << "\n";
        ; // end moves up.
      }
    }
    //    void push(T& element);
    /// Remove oldest element from the queue.
    /**
       \pre queue cannot be empty.
    */
    void pop(void) {
      Node<T>& node = *start;
      if(node.tailp()) {
        node.markEmpty();
      } else {
        start = &node.getNextNode();
        start->setNextNode(*start);
        delete &node;
      }
    }
    size_t size() {
      if(start->emptyp()) {
        return 0;
      } else {
        Node<T>* node = start;
        int i;
        for(i = 1; !node->tailp() && i < 99; i++) {
          node = & (node->getNextNode());
        }
        return i;
      }
      return 10;
    }
  private:
    template<class R>
    class Node {
    public:
      explicit Node(R& element) : element(&element) {
        next_node = this;
      }
      Node(R* element, Node<R>* next)
        : element(element), next_node(next) {}
      /// Node is "empty" if both pointers are null.
      bool emptyp () { return (reinterpret_cast<void*>(&getElement()) == reinterpret_cast<void*>(&getNextNode()));}
      void markEmpty(void) { element = 0; next_node = 0;};
      R& getElement (void) const { return *element; }
      Node<R>& getNextNode(void) const { return *next_node; }
      void setElement (R& element) { this->element = &element; }
      void setNextNode (Node<R>& next_node) { this->next_node = &next_node; }
      bool tailp(void) {
        if(this == next_node) {
          return true;
        } else {
          return false;
        }
      }
    private:
      R* element;
      Node<R>* next_node;
    };
    Node<T>* start;
    Node<T>* end;
  };
}
