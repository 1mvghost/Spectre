#include <acpi.h>
#include <boot.h>
#include <cons.h>
#include <debug.h>
#include <fb.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <limine.h>
#include <mp.h>
#include <panic.h>
#include <pci.h>
#include <pmm.h>
#include <stdio.h>
#include <test.h>
#include <util.h>
#include <vfs.h>
#include <vmm.h>

__attribute__((
    used,
    section(".limine_requests"))) static volatile u64 limine_base_revision[] =
    LIMINE_BASE_REVISION(4);

void main() {
  asm("cli");
  if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == 0) {
    asm("hlt");
  }

  debugInit();

  fbInit();
  consInit();

  printf(0, "spctrx v0.1 (www.github.com/1mvghost/spctrx)\n");
  printf(0, "64-Bit Long Mode (");
  switch (limineFirmwareRequest().response->firmware_type) {
    case LIMINE_FIRMWARE_TYPE_EFI64:
      printf(0, "UEFI)\n\n");
      break;
    case LIMINE_FIRMWARE_TYPE_X86BIOS:
      printf(0, "BIOS)\n\n");
      break;
  }

  gdtInit();
  idtInit();
  isrInit();

  pmmInit();
  vmmInit();

  vfsInit();
  pciInit();
  acpiInit();
  mpInit();

  test();

  keypress();
  acpiReboot();

  panic("\n");
}