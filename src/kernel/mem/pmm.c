#include <pmm.h>
#include <vmm.h>
#include <mmap.h>
#include <debug.h>

/* todo : cleanup ts */
static u64 bmSize;
static u8* bm;

void pmmSet(u64 addr) {
    bm[addr/PAGE_SIZE] = 1;
}
void pmmUnset(u64 addr) {
    bm[addr/PAGE_SIZE] = 0;
}

u64 pmmFind(u64 l) {
    int s=0;
    int j=0;
    for(int i = 0; i<bmSize; i++) {
        if(!bm[i]) {
            s++;
        } else {
            j=i+1;
            s=0;
        }
        if(s==l) {
            return j*PAGE_SIZE;
        }
    }
    return 0x6769420;
}
u64* pmmAlloc(u64 l){
    u64 addr = pmmFind(l);
    if(addr == 0x6769420) panic("OUT OF MEMORY\n");

    u64 p = addr;
    while(l--) {
        pmmSet(p);
        p+=PAGE_SIZE;
    }
    //debug("pmm: alloc %x\n",addr);

    return (u64*)addr;
}

void pmmFree(u64 addr, u64 l) {
    while(l--) {
        pmmUnset(addr);
        addr+=PAGE_SIZE;
    }
}

void pmmInit() {
    //for(int i=0; i < mMapLen(); i++) {
    //    debug("pmm: %x -> %x TYPE:%d SIZE:%d\n", mMapGet(i).base,
    //                                             mMapGet(i).base+mMapGet(i).length,
    //                                             mMapGet(i).type,
    //                                             mMapGet(i).length);
    //}

    u64 total = mMapTotalMem();
    u64 size  = total/PAGE_SIZE;
    bmSize=size;
    debug("pmm: BITMAP SIZE: %x\n",bmSize);

    int ent=-1;
    for(int i=0; i < mMapLen(); i++) {
        if(mMapGet(i).type == 0 && bmSize < mMapGet(i).length) {
            ent=i;
            break;
        }
    }
    if(ent == -1) panic("PMM ERROR :(\n");

    bm = (u8*) VIRT(mMapGet(ent).base);
    memset(bm,0xff,bmSize);

    debug("pmm: BITMAP ADDR: %x\n",bm);

    for(int i=0; i < mMapLen(); i++) {
        if(mMapGet(i).type == 0) {
            u64 b   = mMapGet(i).base;
            u64 len = mMapGet(i).length/PAGE_SIZE;
            while(len--) {
                pmmUnset(b);
                b+=PAGE_SIZE;
            }
        }
    }
    u64 b   = (u64)PHYS(bm);
    u64 len = bmSize/PAGE_SIZE;
    len++; /* just to be safe */
    while(len--){
        pmmSet(b);
        b+=PAGE_SIZE;
    }
}
