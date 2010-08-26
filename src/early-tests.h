#pragma once
#include "types.h"
extern "C" size_t start_tests, end_tests;

namespace tests {
struct TestCase {
  size_t line;
  const char* file;
  const char* function_name;
  bool (*testfunc)();
};
#define T(NAME, X) __attribute__((section ("tests"))) tests::TestCase NAME = { __LINE__, __FILE__, __PRETTY_FUNCTION__, [](){ X;}};

  void runTests(void);
}



