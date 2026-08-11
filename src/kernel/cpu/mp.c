#include <acpi.h>
#include <ahci.h>
#include <alloc.h>
#include <boot.h>
#include <debug.h>
#include <font.h>
#include <gdt.h>
#include <ide.h>
#include <idt.h>
#include <isr.h>
#include <limine.h>
#include <mmap.h>
#include <mp.h>
#include <pci.h>
#include <pmm.h>
#include <stdatomic.h>
#include <stdio.h>
#include <vmm.h>

void mpEntry(struct limine_mp_info* mp) {
  gdtMCpuInit();
  idtMCpuInit();

  debug("cpu%d: ONLINE\n", mp->processor_id);
  asm("cli");
  asm("hlt");
}

void mpInit() {
  struct limine_mp_response* m = limineMpRequest().response;
  debug("mp: found %d cpus\n", m->cpu_count);

  for (u64 i = 0; i < m->cpu_count; i++) {
    atomic_store(&m->cpus[i]->goto_address, &mpEntry);
  }
}