#include "main.h"
#include "experiment.h"
#include "kcommon.h"
#include "library/nlib/lock-free/queue.h"

#define T(NAME, X) __attribute__((section ("tests"))) bool (*NAME)() = [](){ X;};
T(test1, kernel::global::kout << "HIIIAAAASLLLLSLLSLLSLLS"; return true;);
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

  }
}
