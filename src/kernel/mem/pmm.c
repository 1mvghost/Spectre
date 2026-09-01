#include <boot.h>
#include <debug.h>
#include <panic.h>
#include <pmm.h>
#include <vmm.h>

static u64 here = 0;

static u64 used = 0;
static u64 pages = 0;

u64 pmmAlloc(u64 l) {
  if (used >= pages) {
    panic("OUT OF MEMORY\n");
  }
  used += l;
  here += (PAGE_SIZE * l);

  return here - (PAGE_SIZE * l);
}

void pmmFree(u64 addr, u64 l) {
  UNUSED(addr);
  UNUSED(l);
}

void pmmInit() {
  int mMapLen = limineMMapRequest().response->entry_count;

  u64 mx = 0;
  struct limine_memmap_entry* selected;

  for (int i = 0; i < mMapLen; i++) {
    struct limine_memmap_entry* ent = limineMMapRequest().response->entries[i];
    if (ent->type == LIMINE_MEMMAP_USABLE) {
      if (ent->length > mx) {
        mx = ent->length;
        selected = ent;
      }
    }
  }

  here = selected->base;
  pages = mx / PAGE_SIZE;

  debug("pmm: start:%x pages:%d\n", here, pages);
}