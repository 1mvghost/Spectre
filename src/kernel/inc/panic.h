#ifndef PANIC_H
#define PANIC_H

#include <util.h>

struct Stacktrace {
  struct Stacktrace* rbp;
  u64 rip;
};
void panic(char* err);

#endif