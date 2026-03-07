#include <mmap.h>
#include <debug.h>
static u64 total=0;
static int len=0;
/* real hardware fix : increased to 1024 */
static struct limine_memmap_entry ent[1024];

struct limine_memmap_entry mMapGet(u64 i) {
    return ent[i];
}
int mMapLen() {
    return len;
}
u64 mMapTotalMem() {
    return total;
}
void mMapInit(struct limine_memmap_response *mMap) {
    len = mMap->entry_count;
    for(int i=0; i < len; i++) {
        memCpy(&ent[i],mMap->entries[i],sizeof(struct limine_memmap_entry));
    }
    /* last entry has the last possible address */
    total = ent[len-1].base+ent[len-1].length;
}