#include <pmm.h>
#include <vmm.h>
#include <mmap.h>
#include <debug.h>
#include <panic.h>

static u64 here = 0;

static u64 used = 0;
static u64 pages = 0;

u64 pmmAlloc(u64 l){
    if(used >= pages) {
        panic("OUT OF MEMORY\n");
    }
    used += l;
    here += (PAGE_SIZE * l);

    return here - (PAGE_SIZE * l);
}

void pmmFree(u64 addr, u64 l) {}

void pmmInit() {
    //for(int i=0; i < mMapLen(); i++) {
    //    debug("pmm: %x -> %x TYPE:%d SIZE:%d\n", mMapGet(i).base,
    //                                             mMapGet(i).base+mMapGet(i).length,
    //                                             mMapGet(i).type,
    //                                             mMapGet(i).length);
    //}

    u64 mx = 0;
    int selected = 0;
    
    for(int i = 0; i < mMapLen(); i++) {
        if(mMapGet(i).type == LIMINE_MEMMAP_USABLE) {
            if(mMapGet(i).length > mx) {
                mx = mMapGet(i).length;
                selected = i;  
            }
        }
    }

    here = mMapGet(selected).base;
    pages = mx / PAGE_SIZE;

    debug("pmm: start:%x pages:%d\n",here,pages);
}