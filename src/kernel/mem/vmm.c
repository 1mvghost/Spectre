#include <debug.h>
#include <pmm.h>
#include <vmm.h>

#define OFFSET(addr) (((u64)addr) & 0xFFF)
#define P1(addr) (((u64)addr >> 12) & 0x1FF)
#define P2(addr) (((u64)addr >> 21) & 0x1FF)
#define P3(addr) (((u64)addr >> 30) & 0x1FF)
#define P4(addr) (((u64)addr >> 39) & 0x1FF)
#define ATTRIBUTE_SET(ent, attrib) (*ent |= attrib)
#define ATTRIBUTE_CLR(ent, attrib) (*ent &= ~attrib)
#define FRAME_SET(ent, addr)                                                 \
  (*ent =                                                                    \
       (*ent &                                                               \
        ~0b11111111111111111111111111111111111111111111111111000000000000) | \
       addr)
#define PAGE_PHYS_ADDR(p) (p & ~0xFFF)

static PageTable* p4;

void vmmLoad(void* p4);

void* vmmPhysToVirt(u64 phys) {
  return (void*)(phys + 0xffff800000000000);
}

PageTable* vmmGetPageTable(u64 ent) {
  return (PageTable*)vmmPhysToVirt(ent & PAGE_ADDR_MASK);
}

u64 vmmVirtToPhys(void* virt) {
  return ((u64)virt - 0xffff800000000000);
}

void vmmAllocPTE(u64* ent, u64 flag) {
  u64 fr = (u64)pmmAlloc(1);

  memset(vmmPhysToVirt(fr), 0, PAGE_SIZE);

  ATTRIBUTE_SET(ent, PTE_PRESENT);
  ATTRIBUTE_SET(ent, flag);
  FRAME_SET(ent, fr);
}
void vmmMap(void* virt, u64 phys, u64 flag) {
  if (!(p4->Ent[P4(virt)] & PTE_PRESENT))
    vmmAllocPTE(&p4->Ent[P4(virt)], flag);

  PageTable* p3 = (PageTable*)vmmGetPageTable(p4->Ent[P4(virt)]);

  if (!(p3->Ent[P3(virt)] & PTE_PRESENT))
    vmmAllocPTE(&p3->Ent[P3(virt)], flag);

  PageTable* p2 = (PageTable*)vmmGetPageTable(p3->Ent[P3(virt)]);

  if (!(p2->Ent[P2(virt)] & PTE_PRESENT))
    vmmAllocPTE(&p2->Ent[P2(virt)], flag);

  PageTable* p1 = (PageTable*)vmmGetPageTable(p2->Ent[P2(virt)]);

  u64* ent = &p1->Ent[P1(virt)];
  if (!(p1->Ent[P1(virt)] & PTE_PRESENT)) {
    ATTRIBUTE_SET(ent, PTE_PRESENT);
    ATTRIBUTE_SET(ent, flag);
    FRAME_SET(ent, (u64)phys);
  }
}
void vmmMapPages(void* virt, u64 phys, u64 flag, int n) {
  while (n--) {
    vmmMap(virt, phys, flag);
    virt += PAGE_SIZE, phys += PAGE_SIZE;
  }
}
void vmmUnmap(void* virt) {
  if (!(p4->Ent[P4(virt)] & PTE_PRESENT))
    return;

  PageTable* p3 = (PageTable*)vmmGetPageTable(p4->Ent[P4(virt)]);
  if (!(p3->Ent[P3(virt)] & PTE_PRESENT))
    return;

  PageTable* p2 = (PageTable*)vmmGetPageTable(p3->Ent[P3(virt)]);
  if (!(p2->Ent[P2(virt)] & PTE_PRESENT))
    return;

  PageTable* p1 = (PageTable*)vmmGetPageTable(p2->Ent[P2(virt)]);
  u64* ent = &p1->Ent[P1(virt)];
  p1->Ent[P1(virt)] = 0;
  invlpg(ent);
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
  p4 = (PageTable*)vmmPhysToVirt(PAGE_PHYS_ADDR(p4Phys));

  vmmLoad((void*)p4Phys);
}