#include <mmap.h>

static int len=0;
static struct limine_memmap_entry ent[256];

void mMapInit(struct limine_memmap_response *mMap) {
    len = mMap->entry_count;
    for(int i=0; i < len; i++) {
        memCpy(&ent[i],mMap->entries[i],sizeof(struct limine_memmap_entry));
    }
}
struct limine_memmap_entry mMapGet(u64 i) {
    return ent[i];
}
int mMapLen() {
    return len;
}