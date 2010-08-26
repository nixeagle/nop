#include "main.h"
#include "experiment.h"
#include "kcommon.h"
#include "library/nlib/lock-free/queue.h"

namespace experiments {
  void main() {

    using kernel::global::kout;
    bool (*thing[100])(void);
    thing[0] = [](){return (1==1);};
    (thing[0])();

    int a = 323;
    kout << 10 <<  "hi! " << &a << " ooh " << a;
    kernel::global::kout.put("hi\nhi");
    __sync_bool_compare_and_swap(&a, 1, 2);
    __builtin_return_address(0);
    __builtin_frame_address(0);

    // allocate with new to prevent attempts
    nib::Queue<int>& que = *new nib::Queue<int>();
    que.push(a);

    kout << "Queue size: " << que.size() << " Start: " << que.front();
    que.push(a);
    que.pop();
    //    que.pop();
    kout << " size: ";
    kout << que.size();
  }
}
