#include <pmm.h>
#include <vmm.h>

PageTable* p4;

extern void vmmLoad(void* p4);

void* vmmPhysToVirt(u64 phys) {
  return (void*)(phys + 0xffff800000000000);
}

u64 vmmVirtToPhys(void* virt) {
  return ((u64)virt - 0xffff800000000000);
}

PageTable* vmmGetPageTable(u64 ent) {
  return (PageTable*)vmmPhysToVirt(ent & PAGE_ADDR_MASK);
}

u64 vmmInitPTE(u64 frame, u64 flags) {
  u64 ent = 0;

  ATTRIBUTE_SET(&ent, PTE_PRESENT);
  ATTRIBUTE_SET(&ent, flags);
  FRAME_SET(&ent, frame);

  return ent;
}

u64 vmmAllocPTE(u64 flags) {
  return vmmInitPTE(pmmAlloc(1), flags);
}

PageTable* vmmWalk(void* virt, bool alloc, u64 allocFlags) {
  if (p4->Ent[P4(virt)] == 0) {
    if (!alloc)
      return 0;
    p4->Ent[P4(virt)] = vmmAllocPTE(allocFlags);
  }

  PageTable* p3 = vmmGetPageTable(p4->Ent[P4(virt)]);

  if (p3->Ent[P3(virt)] == 0) {
    if (!alloc)
      return 0;
    p3->Ent[P3(virt)] = vmmAllocPTE(allocFlags);
  }

  PageTable* p2 = vmmGetPageTable(p3->Ent[P3(virt)]);

  if (p2->Ent[P2(virt)] == 0) {
    if (!alloc)
      return 0;
    p2->Ent[P2(virt)] = vmmAllocPTE(allocFlags);
  }

  return vmmGetPageTable(p2->Ent[P2(virt)]);
}
void vmmMap(void* virt, u64 phys, u64 flags) {
  PageTable* p1 = vmmWalk(virt, true, flags);

  p1->Ent[P1(virt)] = vmmInitPTE(phys, flags);
  invlpg(&p1->Ent[P1(virt)]);
}

void vmmMapPages(void* virt, u64 phys, u64 flags, int n) {
  while (n--) {
    vmmMap(virt, phys, flags);
    virt += PAGE_SIZE, phys += PAGE_SIZE;
  }
}

void vmmUnmap(void* virt) {
  PageTable* p1 = vmmWalk(virt, false, 0);
  if (p1 == 0)
    return;

  p1->Ent[P1(virt)] = 0;
  invlpg(&p1->Ent[P1(virt)]);
}

void vmmUnmapPages(void* virt, int n) {
  while (n--) {
    vmmUnmap(virt);
    virt += PAGE_SIZE;
  }
}

void* vmmAlloc(u64 pages) {
  return vmmPhysToVirt(pmmAlloc(pages));
}

void vmmInit() {
  /* copy page tables from limine, there is no need to make new ones because
   * limine does everything beautifully */

  u64 p4Phys = 0;
  asm("movq %%cr3, %0" : "=r"(p4Phys));
  p4 = (PageTable*)vmmPhysToVirt(p4Phys);

  vmmLoad((void*)p4Phys);
}