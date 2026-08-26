#include <acpi.h>
#include <ahci.h>
#include <alloc.h>
#include <debug.h>
#include <fb.h>
#include <font.h>
#include <gdt.h>
#include <ide.h>
#include <idt.h>
#include <isr.h>
#include <limine.h>
#include <ll.h>
#include <mem.h>
#include <mp.h>
#include <ops.h>
#include <pci.h>
#include <pmm.h>
#include <stdio.h>
#include <util.h>
#include <vmm.h>

void test() {
// #define IDE_TEST
#ifdef IDE_TEST
  printf(INFO, "ide: test\n");
  u8* buf = calloc(512);
  ideRead(0, 0, 1, buf);
  for (int i = 0; i < 512; i++)
    printf(0, "%c", buf[i]);
  free(buf);
  printf(0, "\n");
#endif
// #define AHCI_TEST
#ifdef AHCI_TEST
  u8* buf = calloc(512);
  printf(INFO, "ahci: test\n");
  buf = calloc(512);
  ahciRead(0, 0, 1, buf);
  for (int i = 0; i < 512; i++)
    printf(0, "%c", buf[i]);
  free(buf);
  printf(0, "\n");
#endif
#ifdef SERIAL_TEST
  // debugPuts("serial test test\n");
  debug("serial printf test %d %d %d %x %x", 67, 69, 420, 0xdeadbeef,
        0xcafebabe);
#endif
#ifdef PMM_TEST
  u64* a = pmmAlloc(1);
  u64* b = pmmAlloc(1);
  printf(INFO, "a:%x b:%x\n", a, b);

  pmmFree(a, 1);

  pmmAlloc(2);
  pmmAlloc(3553323);

#endif
#ifdef PRINT_TEST
  printf(INFO, "test\n");
  char buf[67];
  memset(buf, 0, 67);
  sprintf(buf, "Hello world %x %x hi 123 %s %c\ns\n", 0xdeadbeef, 0xcafebabe,
          "string", '!');
  for (int i = 0; i < 67; i++) {
    printf(0, "%c", buf[i]);
  }
  printf(0, "hi!\n");
#endif
// #define ALLOC_TEST
#ifdef ALLOC_TEST
  void* t[4096];
  for (int i = 0; i < 830; i++) {
    t[i] = calloc(4096);
  }
  spdmp();
  for (int i = 0; i < 4096; i++) {
    free(t[i]);
  }
  spdmp();
#endif
#define VFS_TEST
#ifdef VFS_TEST
  struct FsFd* fd = vfsOpen("/dev/dbg", 0x67);
  if (!fd) {
    return;
  }

  char* buf = "SERIAL TEST\n";
  printf(INFO, "vfs: wrote %d bytes\n", vfsWrite(fd, (u8*)buf, strlen(buf)));

  printf(INFO, "* readdir on /dev *\n");
  struct FsFd* fd2 = vfsOpen("/dev", 0x03);
  void* testt = calloc(1024);
  struct linux_dirent64* dent = testt;

  vfsReadDir(fd2, dent, 1);
  printf(INFO, "%s\n", dent->d_name);

  printf(INFO, "* readdir on / *\n");
  struct FsFd* fd3 = vfsOpen("/", 0x03);
  void* testtt = calloc(1024);
  struct linux_dirent64* dentt = testtt;

  vfsReadDir(fd3, dentt, 1);

  printf(INFO, "%s\n", dentt->d_name);

#endif
#ifdef CONS_TEST
  for (int i = 0; i < 100000; i++) {
    printf(0, "abb");
  }
#endif
#ifdef LL_TEST
  LinkedList ll;
  llInit(&ll);

  llAdd(&ll, "Hello World test");
  llAdd(&ll, "Linked list test 123");
  llAdd(&ll, "Linked list test 123");
  llAdd(&ll, "Linked list test 123");
  llAdd(&ll, "Linked list test 123");
  llAdd(&ll, "Linked list test 123");
  llAdd(&ll, "Linked list test 123");

  LLNode* cur = ll.Head;
  while (cur) {
    printf(INFO, "%s\n", cur->Data);
    cur = cur->Next;
  }
#endif
}