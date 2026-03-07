#ifndef PMM_H
#define PMM_H

#include <util.h>
u64* pmmAlloc(u64 b);
void pmmInit(u64 start, u64 end);
#endif