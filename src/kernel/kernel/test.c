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
#include <ops.h>

void test() {
//#define IDE_TEST
#ifdef IDE_TEST
   printf(INFO,"ide: test\n");
   u8* buf = calloc(512);
   ideRead(0,0,1,buf);
   for(int i = 0; i < 512; i++) printf(0,"%c",buf[i]);
   free(buf);
   printf(0,"\n");
#endif
//#define AHCI_TEST
#ifdef AHCI_TEST
   u8* buf = calloc(512);
   printf(INFO,"ahci: test\n");
   buf = calloc(512);
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
//#define ALLOC_TEST
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
#define VFS_TEST
#ifdef VFS_TEST
   struct FsFd *fd = vfsOpen("/dev/dbg", 0x67);
   if(!fd) {
      return;
   }
   
   char* buf = "SERIAL TEST\n";
   printf(INFO,"vfs: wrote %d bytes",vfsWrite(fd,(u8*)buf,strlen(buf)));
#endif
}