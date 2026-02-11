#include <pmm.h>
#include <vmm.h>

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
}
