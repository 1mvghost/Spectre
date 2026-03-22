#ifndef KERNEL_H
#define KERNEL_H

struct Stacktrace{
   struct Stacktrace* rbp;
   u64                rip;
};
void panic(char* err);

#endif