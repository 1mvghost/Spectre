#include <panic.h>
#include <stdio.h>
#include <util.h>

void panic(char* err) {
  u32 eax = 1, ebx, ecx, edx;
  cpuid(&eax, &ebx, &ecx, &edx);
  u32 cpuId = (ebx >> 24) & 0xFF;

  printf(PANIC, "%s", err);
  printf(PANIC, "CPU: %d\n", cpuId);
  printf(PANIC, "--- Kernel Call Trace ---\n");

  struct Stacktrace* stk;
  asm("movq %%rbp,%0" : "=r"(stk)::);

  for (u64 fr = 0; stk && fr < 10; ++fr) {
    if (stk->rip == 0)
      break;
    printf(PANIC, "%x\n", stk->rip);
    stk = stk->rbp;
  }

  asm("cli");
  asm("hlt");
}