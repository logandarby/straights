#include <iostream>

#include "debug.h"

void Debug::print(const std::string msg) {
  #if DEBUG
    std::cout << "\u001b[4m\u001b[36m\u001b[1mDebug:\u001b[0m "
              << msg << std::endl;
  #endif
  return;
}

