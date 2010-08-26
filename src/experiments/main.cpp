#include "main.h"
#include "experiment.h"
#include "kcommon.h"
#include "library/nlib/lock-free/queue.h"

#define TEST(X) experiments::tests->push([](){ X; });

namespace experiments {
  nib::Queue<bool (*)()>* tests = 0;
  nib::Queue<int> asas = *new nib::Queue<int>;
  void main() {
    tests = new nib::Queue<bool (*)()>;

    using kernel::global::kout;
    bool (*thing[100])(void);
    thing[0] = [](){kout << "it worked!" << "\n\n"; return true;};
    (thing[0])();

    int a = 323;
    kout << 10 <<  "hi! " << &a << " ooh " << a;
    kernel::global::kout.put("hi\nhi");
    __sync_bool_compare_and_swap(&a, 1, 2);
    __builtin_return_address(0);
    __builtin_frame_address(0);



    //    bool (*c)(void) = []() -> bool {return true;};
    TEST(kout << "ASAAAA"; return true;)
    tests->push( []() {
        // nib::Queue<int> que;
        // int a = 323;
        // int b = 987;
        // que.push(a);
        // que.push(b);
        // kout << " aaaasize: " << que.size() << "  " <<  __builtin_return_address(0) << "  " << que.front();
        // que.pop();
        // kout << " size now: " << que.size() << " value: " << que.front();
        return true;
      });
    tests->push( [](){kout << "STILL ALIVE!"; return false;});
    (tests->front())();
    tests->pop();
    (tests->front())();
    tests->pop();
    (tests->front())();
    tests->pop();
    //(tests->front())();
    //    tests->pop();
    //    (tests->front())();
    kout << "\n" << "SIZE: " << tests->size() + 1;
  }
}
