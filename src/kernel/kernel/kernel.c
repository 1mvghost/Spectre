#include <util.h>
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
#include <mp.h>
#include <mem.h>
#include <fb.h>
#include <vfs.h>
#include <test.h>

__attribute__((used, section(".limine_requests")))
static volatile u64 limine_base_revision[] = LIMINE_BASE_REVISION(4);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebufferRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request mMapRequest = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0
};
__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdpRequest = {
    .id = LIMINE_RSDP_REQUEST_ID,
    .revision = 4
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_firmware_type_request firmwareRequest = {
    .id = LIMINE_FIRMWARE_TYPE_REQUEST_ID,
    .revision = 4
};
__attribute__((used, section(".limine_requests")))
static volatile struct limine_module_request modRequest = {
    .id = LIMINE_MODULE_REQUEST_ID,
    .revision = 4
};
__attribute__((used, section(".limine_requests")))
static volatile struct limine_mp_request mpRequest = {
    .id = LIMINE_MP_REQUEST_ID,
    .revision = 4
};

__attribute__((used, section(".limine_requests_start")))
static volatile u64 limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile u64 limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

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
   struct limine_framebuffer *fb = framebufferRequest.response->framebuffers[0];
   struct limine_mp_response *mp = mpRequest.response;

   u64 fbAddr =   (u64)fb->address;
   u64 fbPitch =  (u64)fb->pitch;
   u64 fbHeight = (u64)fb->height;
   u64 acpiAddr = 0;

   if(rsdpRequest.response) {
      acpiAddr = (u64)rsdpRequest.response->address;
   }

   debugInit();
   gdtInit();
   idtInit();
   isrInit();
   fbInit(fbPitch,fbHeight,fbAddr);

   printf(0,"Spectre v1.0 (www.github.com/1mvghost/Spectre)\n");
   printf(0,"64-Bit Long Mode ("); 
   switch(firmwareRequest.response->firmware_type) {
      case LIMINE_FIRMWARE_TYPE_EFI64:   printf(0,"UEFI)\n\n"); break;
      case LIMINE_FIRMWARE_TYPE_X86BIOS: printf(0,"BIOS)\n\n"); break;
   }

   mMapInit(mMapRequest.response);
   pmmInit();
   vmmInit();
   vfsInit();
   pciInit();
   acpiInit(acpiAddr);
   mpInit(mp);
   
   test();

   keypress();
   acpiReboot();

   panic("\n");
}