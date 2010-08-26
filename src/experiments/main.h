#pragma once
#include "library/nlib/lock-free/queue.h"

/** \file main.h primary entry point from kernel -> experimentation code */

namespace experiments {
  extern nib::Queue<bool (*)()>* tests;
  void main(void);
}
