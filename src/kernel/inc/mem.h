#ifndef MEM_H
#define MEM_H
#include <stdatomic.h>
#include <util.h>

void mSpinlockAcquire(atomic_flag *l);
void mSpinlockDrop(atomic_flag *l);

#endif