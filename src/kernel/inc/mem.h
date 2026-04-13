#ifndef MEM_H
#define MEM_H
#include <stdatomic.h>
#include <util.h>

/* make stuff more readable */
typedef atomic_flag Splock;

void mSpinlockAcquire(Splock *l);
void mSpinlockDrop(Splock *l);

#endif