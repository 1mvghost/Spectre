#include <pmm.h>
#include <vmm.h>
#include <mmap.h>
#include <debug.h>

static u64 bmSize;
static u8* bm;

void pmmSet(u64 addr) {
    bm[addr/4096] = 1;
}
void pmmUnset(u64 addr) {
    bm[addr/4096] = 0;
}

u64 pmmFind() {
    for(int i = 0; i<bmSize; i++) {
        if(!bm[i]) {
            return i*4096;
        }
    }
    return 0x6769420;
}
u64* pmmAlloc(){
    u64 addr = pmmFind();
    if(addr == 0x6769420) panic("OUT OF MEMORY\n");

    pmmSet(addr);

    debug("pmm: alloc %x\n",addr);

    return addr;
}

void pmmFree(u64 addr) {
    pmmUnset(addr);
}

void pmmInit() {
    for(int i=0; i < mMapLen(); i++) {
        debug("pmm: %x -> %x TYPE:%d SIZE:%d\n", mMapGet(i).base,
                                                 mMapGet(i).base+mMapGet(i).length,
                                                 mMapGet(i).type,
                                                 mMapGet(i).length);
    }

    u64 total = mMapTotalMem();
    u64 size  = total/4096/8;

    bmSize=size;
    debug("pmm: BITMAP SIZE: %x\n",bmSize);

    int ent=-1;
    for(int i=0; i < mMapLen(); i++) {
        if(mMapGet(i).type == 0 && bmSize <= mMapGet(i).length) {
            ent=i;
            break;
        }
    }
    if(ent == -1) panic("PMM ERROR :(\n");

    bm = VIRT(mMapGet(ent).base);
    memSet(bm,0xff,bmSize);

    debug("pmm: BITMAP ADDR: %x\n",bm);

    for(int i=0; i < mMapLen(i); i++) {
        if(mMapGet(i).type == 0) {
            u64 b   = mMapGet(i).base;
            u64 len = mMapGet(i).length/4096;
            while(len--) {
                pmmUnset(b);
                b+=4096;
            }
        }
    }
    u64 b   = PHYS(bm);
    u64 len = bmSize/4096;
    len++; /* just to be safe */
    while(len--){
        pmmSet(b);
        b+=4096;
    }
}
