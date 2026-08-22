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

void vmmAllocPTE(u64* ent, u64 flags) {
  u64 fr = (u64)pmmAlloc(1);

  memset(vmmPhysToVirt(fr), 0, PAGE_SIZE);

  ATTRIBUTE_SET(ent, PTE_PRESENT);
  ATTRIBUTE_SET(ent, flags);
  FRAME_SET(ent, fr);
}

void vmmMap(void* virt, u64 phys, u64 flags) {
  if (p4->Ent[P4(virt)] == 0)
    vmmAllocPTE(&p4->Ent[P4(virt)], flags);

  PageTable* p3 = (PageTable*)vmmGetPageTable(p4->Ent[P4(virt)]);

  if (p3->Ent[P3(virt)] == 0)
    vmmAllocPTE(&p3->Ent[P3(virt)], flags);

  PageTable* p2 = (PageTable*)vmmGetPageTable(p3->Ent[P3(virt)]);

  if (p2->Ent[P2(virt)] == 0)
    vmmAllocPTE(&p2->Ent[P2(virt)], flags);

  PageTable* p1 = (PageTable*)vmmGetPageTable(p2->Ent[P2(virt)]);

  u64* ent = &p1->Ent[P1(virt)];

  invlpg(ent);

  ATTRIBUTE_SET(ent, PTE_PRESENT);
  ATTRIBUTE_SET(ent, flags);
  FRAME_SET(ent, (u64)phys);
}

void vmmMapPages(void* virt, u64 phys, u64 flags, int n) {
  while (n--) {
    vmmMap(virt, phys, flags);
    virt += PAGE_SIZE, phys += PAGE_SIZE;
  }
}

void vmmUnmap(void* virt) {
  if (p4->Ent[P4(virt)] == 0)
    return;

  PageTable* p3 = (PageTable*)vmmGetPageTable(p4->Ent[P4(virt)]);

  if (p3->Ent[P3(virt)] == 0)
    return;

  PageTable* p2 = (PageTable*)vmmGetPageTable(p3->Ent[P3(virt)]);

  if (p2->Ent[P2(virt)] == 0)
    return;

  PageTable* p1 = (PageTable*)vmmGetPageTable(p2->Ent[P2(virt)]);

  invlpg(&p1->Ent[P1(virt)]);
  p1->Ent[P1(virt)] = 0;
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