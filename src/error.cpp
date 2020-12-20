#include <iostream>
#include "error.h"

void error_message(const char * const msg) {
  std::cerr << msg << "\n";
}
