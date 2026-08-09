#ifndef VMM_H
#define VMM_H

#define PAGE_SIZE           0x1000 //4096
#define PAGE_ADDR_MASK      0x000ffffffffff000
#define	PTE_PRESENT			1		    //0000000000000000000000000000001
#define	PTE_WRITABLE		2		    //0000000000000000000000000000010
#define	PTE_USER			4		    //0000000000000000000000000000100
#define	PTE_WRITETHOUGH		8		    //0000000000000000000000000001000
#define	PTE_NOT_CACHEABLE   0x10		//0000000000000000000000000010000
#define	PTE_ACCESSED		0x20		//0000000000000000000000000100000
#define	PTE_DIRTY			0x40		//0000000000000000000000001000000
#define	PTE_HUGE		    0x80		//0000000000000000000000010000000
#define	PTE_CPU_GLOBAL		0x100		//0000000000000000000000100000000
#define	PTE_LV4_GLOBAL		0x200		//0000000000000000000001000000000
#define PTE_FRAME			0x7FFFF000 	//0000000000000000000000000000000000000000000000000000111111111111

#include <util.h>

typedef struct {
    u64 Ent[512];
} PageTable;

void vmmInit();

void vmmMapPages(void* virt, u64 phys, u64 flag, int n);
void vmmUnmapPages(void *virt, int n);

void vmmMap(void* virt, u64 phys, u64 flag);
void vmmUnmap(void* virt);

u64 vmmVirtToPhys(void* virt);
void* vmmPhysToVirt(u64 phys);

void* vmmAlloc(u64 pages);

#endif