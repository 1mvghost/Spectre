#include <mem.h>

void mSpinlockAcquire(Splock *l) {
    while(atomic_flag_test_and_set_explicit(l, memory_order_acquire)) {

    }
}

void mSpinlockDrop(Splock *l) {
    atomic_flag_clear_explicit(l,memory_order_release);
}