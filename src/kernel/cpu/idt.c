#include <idt.h>
#include <vmm.h>
#include <kernel.h>
typedef struct {
    u16 OffsetLow;
    u16 Selector;
    u8  Ist;
    u8  Attributes;
    u16 OffsetMid;
    u32 OffsetHigh;
    u32 Reserved;
} __attribute__((packed)) IDTEntry;

typedef struct {
    u16       Limit;
    IDTEntry* Base;
} __attribute__((packed)) IDTR;

__attribute__((aligned(0x10)))
static IDTEntry idt[256];
static IDTR     idtr;

extern void idtLoad(IDTR *idtr);


void idtSetDesc(u8 i, void* isr, u8 flags) {
    IDTEntry* entry = &idt[i];
    entry->OffsetLow  = (u64)isr & 0xFFFF;
    entry->Selector   = 0x08;
    entry->Attributes = flags;
    entry->OffsetMid  = ((u64)isr>>16) & 0xFFFF;
    entry->OffsetHigh = ((u64)isr>>32) & 0xFFFFFFFF;
    entry->Reserved   = 0;
    entry->Ist        = 0;
}
void idtInit() {
    idtr.Base =  (u64)idt;
    idtr.Limit = sizeof(idt)-1;
    
    idtLoad(&idtr);
}
void idtMCpuInit() {
    idtLoad(&idtr);
}