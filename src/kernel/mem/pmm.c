#include <pmm.h>
#include <vmm.h>
#include <mmap.h>

/* best memory allocator of all time */
static u64 mEnd;
static u64 addr;

u64* pmmAlloc(u64 b){
    if(addr >= mEnd){
        panic("OUT OF MEMORY\n");
        return;
    }
    addr += b;
    return (void*) addr;

}

void pmmInit(u64 start, u64 end) {
    addr=start; mEnd=end;

    for(int i=0; i < mMapLen(); i++) {
        debug("pmm: %x -> %x TYPE:%d SIZE:%d\n", mMapGet(i).base,
                                                 mMapGet(i).base+mMapGet(i).length,
                                                 mMapGet(i).type,
                                                 mMapGet(i).length);
    }
}

