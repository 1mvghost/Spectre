#include <gdt.h>

typedef struct {
    u16 LimitLow;
    u16 BaseLow;
    u8  BaseMid;
    u8  Access;
    u8  LimitHigh:4;
    u8  Flags:4;
    u8  BaseHigh;
} __attribute__((packed)) GDTEntry;

typedef struct {
    u16       Limit;
    GDTEntry* Base;
} __attribute__((packed)) GDTR;

__attribute__((aligned(0x08)))
static GDTEntry gdt[6];
static GDTR     gdtr;

extern void gdtLoad(GDTR *gdtr);
extern void segReload();

void gdtSetDesc(u8 i, u32 limit, u32 base, u8 access, u8 flags) {
    GDTEntry* entry = &gdt[i];
    entry->BaseLow      = base & 0xffff;
    entry->BaseMid      = (base >> 16) & 0xff;
    entry->BaseHigh     = (base >> 24) & 0xff;
    entry->Access       = access;
    entry->LimitLow     = limit & 0xffff;
    entry->LimitHigh    = (limit >> 16) & 0xf;
    entry->Flags        = flags;
    
}

void gdtInit() {
    /* todo: tss */
    memset(gdt,0,sizeof(gdt));
    gdtr.Base =  gdt;
    gdtr.Limit = sizeof(gdt)-1;

    gdtSetDesc(0,0,0,0,0);
    gdtSetDesc(1,0x00ffffff,0,0x9a,0xa);
    gdtSetDesc(2,0x00ffffff,0,0x92,0xc);
    gdtSetDesc(3,0x00ffffff,0,0xfa,0xa);
    gdtSetDesc(4,0x00ffffff,0,0xf2,0xc);

    gdtLoad(&gdtr);
    segReload(); /* change code to 0x08 and data to 0x10 */
}

void gdtMCpuInit() {
    gdtLoad(&gdtr);
    segReload(); /* change code to 0x08 and data to 0x10 */
}