#include <vmm.h>
#include <pmm.h>


#define OFFSET(addr)              (addr&0xFFF)
#define P1(addr)                  ((addr >> 12)&0x1FF)
#define P2(addr)                  ((addr >> 21)&0x1FF)
#define P3(addr)                  ((addr >> 30)&0x1FF)
#define P4(addr)                  ((addr >> 39)&0x1FF)
#define ATTRIBUTE_SET(ent,attrib) (*ent |= attrib)
#define ATTRIBUTE_CLR(ent,attrib) (*ent &= ~attrib)
#define FRAME_SET(ent,addr)       (*ent = (*ent & ~0b11111111111111111111111111111111111111111111111111000000000000) | addr)
#define PAGE_PHYS_ADDR(p)         (p & ~0xFFF)

typedef struct {
    u64 Ent[512];
} PageTable;

static PageTable* p4;

void vmmLoad(u64 *p4);

void vmmMap(u64 virt, u64 phys, u64 n, u64 flag) {
    while (n--) {
        if(!(p4->Ent[P4(virt)] & PTE_PRESENT)) {
            u64* ent = &p4->Ent[P4(virt)];
            u64* fr = (u64*) pmmAlloc(1);
            memset(VIRT(fr),0,4096);
            ATTRIBUTE_SET(ent, PTE_PRESENT);
            ATTRIBUTE_SET(ent, flag);
            FRAME_SET    (ent, (u64)fr);

        }
        PageTable* p3 = (PageTable*) VIRT(p4->Ent[P4(virt)] & PAGE_ADDR_MASK);
        if(!(p3->Ent[P3(virt)] & PTE_PRESENT)) {
            u64* ent = &p3->Ent[P3(virt)];
            u64* fr = (u64*) pmmAlloc(1);
            memset(VIRT(fr),0,4096);
            ATTRIBUTE_SET(ent, PTE_PRESENT);
            ATTRIBUTE_SET(ent, flag);
            FRAME_SET    (ent, (u64)fr);
        }
        PageTable* p2 = (PageTable*) VIRT(p3->Ent[P3(virt)] & PAGE_ADDR_MASK);
        if(!(p2->Ent[P2(virt)] & PTE_PRESENT)) {
            u64* ent = &p2->Ent[P2(virt)];
            u64* fr = (u64*) pmmAlloc(1);
            memset(VIRT(fr),0,4096);
            ATTRIBUTE_SET(ent, PTE_PRESENT);
            ATTRIBUTE_SET(ent, flag);
            FRAME_SET    (ent, (u64)fr);
        }
        PageTable* p1 = (PageTable*) VIRT(p2->Ent[P2(virt)] & PAGE_ADDR_MASK);
        u64* ent = &p1->Ent[P1(virt)];
        if(!(p1->Ent[P1(virt)] & PTE_PRESENT)) {
            ATTRIBUTE_SET(ent, PTE_PRESENT);
            ATTRIBUTE_SET(ent, flag);
            FRAME_SET    (ent, (u64)phys);
        }
        phys+=PAGE_SIZE; virt+=PAGE_SIZE;
    }
}
void vmmUnmap(u64 virt, u64 n) {
    while (n--) {
        if(!(p4->Ent[P4(virt)] & PTE_PRESENT)) {
            virt+=PAGE_SIZE;
            continue;
        }
        PageTable* p3 = (PageTable*) VIRT(p4->Ent[P4(virt)] & PAGE_ADDR_MASK);
        if(!(p3->Ent[P3(virt)] & PTE_PRESENT)) {
            virt+=PAGE_SIZE;
            continue;
        }
        PageTable* p2 = (PageTable*) VIRT(p3->Ent[P3(virt)] & PAGE_ADDR_MASK);
        if(!(p2->Ent[P2(virt)] & PTE_PRESENT)) {
            virt+=PAGE_SIZE;
            continue;
        }
        PageTable* p1 = (PageTable*) VIRT(p2->Ent[P2(virt)] & PAGE_ADDR_MASK);
        u64* ent = &p1->Ent[P1(virt)];
        p1->Ent[P1(virt)] = 0;
        invlpg(ent);
        virt+=PAGE_SIZE;
        continue;
    }
    
}
void vmmInit(){
    /* page table copy */
    u64 p4p = 0;
    asm("movq %%cr3, %0" : "=r"(p4p));
    p4 = (PageTable*)VIRT(PAGE_PHYS_ADDR(p4p));

    vmmLoad(PHYS((u64)p4));
}

