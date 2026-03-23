#include <mp.h>
#include <stdio.h>
#include <acpi.h>
#include <pci.h>
#include <ide.h>
#include <idt.h>
#include <isr.h>
#include <pmm.h>
#include <vmm.h>
#include <font.h>
#include <acpi.h>
#include <limine.h>
#include <gdt.h>
#include <ahci.h>
#include <debug.h>
#include <mmap.h>
#include <alloc.h>
#include <stdatomic.h>

void mpEntry(struct limine_mp_info *mp) {
    gdtMCpuInit();
    idtMCpuInit();
    debug("cpu%d: ONLINE\n",mp->processor_id);   
    asm("cli");asm("hlt");
}

void mpInit(struct limine_mp_response *m) {
    debug("mp: found %d cpus\n",m->cpu_count);
    for(int i = 0; i<m->cpu_count; i++) {
        atomic_store(&m->cpus[i]->goto_address,&mpEntry);    
    }
}