#include "early-tests.h"
#include "kernel/virtual-console/virtual-console.h"

namespace tests {
  size_t total_tests_run = 0;
  size_t total_tests_passed = 0;
  size_t total_tests_failed = 0;
  void runTests(void) {
    for(TestCase *test = reinterpret_cast<TestCase*>(&start_tests); test < reinterpret_cast<TestCase*>(&end_tests); test++) {
      total_tests_run++;
      //    ((bool (*) (void))(*test))();
      if(test->testfunc()) {
        total_tests_passed++;
      } else {
        total_tests_failed++;
        using kernel::global::testout;
        testout << "("
                << static_cast<size_t>(total_tests_run) << ", "
                << static_cast<size_t>(total_tests_passed) << ", "
                << static_cast<size_t>(total_tests_failed) << ") "
                << test->file << ":"
                << static_cast<size_t>(test->line) << " "
                << test->function_name << " failed.\n";
      }
    }
  }
}
