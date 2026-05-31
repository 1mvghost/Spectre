#include <util.h>
#include <limine.h>
#include <debug.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <fb.h>
#include <vfs.h>
#include <stdio.h>
#include <mmap.h>
#include <pci.h>
#include <acpi.h>
#include <mp.h>
#include <test.h>
#include <boot.h>
#include <pmm.h>
#include <vmm.h>
#include <cons.h>

__attribute__((used, section(".limine_requests")))
static volatile u64 limine_base_revision[] = LIMINE_BASE_REVISION(4);

void panic(char* err) {
   u32 eax=1,ebx,ecx,edx;
   cpuid(&eax,&ebx,&ecx,&edx);
   u32 cpuId = (ebx>>24) & 0xFF; 

   debug("panic: %s",err);
   debug("panic: CPU: %d\n",cpuId);
   debug("panic: --- Kernel Call Trace ---\n");

   struct Stacktrace *stk;
   asm("movq %%rbp,%0" : "=r"(stk) ::);

   for(u64 fr = 0; stk && fr < 10; ++fr) {
      if(stk->rip==0) break;
      debug("panic: %x\n",stk->rip);
      stk = stk->rbp;
   }
   
   asm("cli"); asm("hlt");
}

void main(){
   asm("cli");
   if(LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == 0) {
      asm("hlt");
   }   
   
   debugInit();
   gdtInit();
   idtInit();
   isrInit();
   fbInit();
   consInit();

   printf(0,"Spectre v1.0 (www.github.com/1mvghost/Spectre)\n");
   printf(0,"64-Bit Long Mode ("); 
   switch(limineFirmwareRequest().response->firmware_type) {
      case LIMINE_FIRMWARE_TYPE_EFI64:   printf(0,"UEFI)\n\n"); break;
      case LIMINE_FIRMWARE_TYPE_X86BIOS: printf(0,"BIOS)\n\n"); break;
   }
   
   mMapInit();
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