#ifndef _H_DEBUG
#define _H_DEBUG

/*
 A debug console which can be switched on by compiling
 with the -DDEBUG=1 flag
*/

#include <string>

#ifndef DEBUG
#define DEBUG 0
#endif

class Debug {
  // Is a static class
  Debug() {};
public:
  static void print(std::string msg);
};

#endif
