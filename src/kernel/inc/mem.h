#ifndef MEM_H
#define MEM_H
#include <stdatomic.h>
#include <util.h>

void mSpinlockAcquire(int *l);
void mSpinlockDrop(int *l);

#endif