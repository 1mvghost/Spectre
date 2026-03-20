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


__attribute__((used, section(".limine_requests_start")))
static volatile u64 limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile u64 limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;


struct Stacktrace{
   struct Stacktrace* rbp;
   u64                rip;
};

void panic(char* err) {
   printf(PANIC,"%s",err);
   printf(PANIC,"--- Kernel Call Trace ---\n");
   struct Stacktrace *stk;
   asm("movq %%rbp,%0" : "=r"(stk) ::);
   //while(stk) {
   for(u64 fr = 0; stk && fr < 10; ++fr) {
      if(stk->rip==0) break;
      printf(PANIC,"%x\n",stk->rip);
      stk = stk->rbp;
   }
   asm("cli"); asm("hlt");
}

void test() {
#ifdef IDE_TEST
   debug("ide: test\n");
   u8* buf = calloc(512);
   ideRead(0,0,1,buf);
   for(int i = 0; i < 512; i++) printf(0,"%c",buf[i]);
   free(buf);
   printf(0,"\n");
#endif
#ifdef AHCI_TEST
   u8* buf = calloc(512);
   ahciRead(0,0,1,buf);
   for(int i = 0; i < 512; i++) printf(0,"%c",buf[i]);
   free(buf);
   printf(0,"\n");
#endif
#ifdef SERIAL_TEST
   //debugPuts("serial test test\n");
   debug("serial printf test %d %d %d %x %x",67,69,420,0xdeadbeef,0xcafebabe);
#endif
#ifdef PMM_TEST
   u64 *a = pmmAlloc(1);
   u64 *b = pmmAlloc(1);
   printf(INFO,"a:%x b:%x\n",a,b);

   pmmFree(a,1);

   pmmAlloc(2);
   pmmAlloc(3553323);

#endif
#ifdef PRINT_TEST
   printf(INFO,"test\n");
   char buf[67];
   memset(buf,0,67);
   sprintf(buf,"Hello world %x %x hi 123 %s %c\ns\n",0xdeadbeef,0xcafebabe,"string",'!');
   for(int i=0; i<67; i++) {
      printf(0,"%c",buf[i]);
   }
   printf(0,"hi!\n");
#endif
#ifdef ALLOC_TEST
   void* t[4096];
   for(int i = 0; i < 830; i++) {
      t[i]=calloc(4096);
   }
   spdmp();
   for(int i = 0; i < 4096; i++) {
      free(t[i]);
   }
   spdmp();
#endif
}

void main(){
   asm("cli");
   if(LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == 0) {
      asm("hlt");
   }
   struct limine_framebuffer *fb = framebufferRequest.response->framebuffers[0];
   struct limine_memmap_entry* mMap = mMapRequest.response->entries[0];

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
   pciInit();
   acpiInit(acpiAddr);
    
   test();
   
   keypress();
   acpiShutdown();

   panic("\n");
}