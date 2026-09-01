#include <panic.h>
#include <printf.h>
#include <stdarg.h>
#include <util.h>
static const char* exceptions[32] = {"Div By Zero",
                                     "Debug",
                                     "NMI",
                                     "Breakpoint",
                                     "Overflow",
                                     "Bound Range Exceeded",
                                     "Invalid Opcode",
                                     "Device Not Available",
                                     "Double Fault",
                                     "Coprocessor Segment Overrun",
                                     "Bad TSS",
                                     "Segment Not Present",
                                     "Stack-Segment Fault",
                                     "General Protection Fault",
                                     "Page Fault",
                                     "Unknown",
                                     "x87 Floating-Point",
                                     "Alignment Check",
                                     "Machine Check",
                                     "Unknown",
                                     "Unknown",
                                     "Unknown",
                                     "Unknown",
                                     "Unknown",
                                     "Unknown",
                                     "Unknown",
                                     "Unknown",
                                     "Unknown",
                                     "Unknown",
                                     "Unknown"};

void panicIsr(Regs* regs) {
  printf("%s\n", exceptions[regs->intId]);
  printf("STOP:%x INT:%x\n", regs->errId, regs->intId);
  printf("RAX:%x RBX:%x RCX:%x RDX:%x RSP:%x RDI:%x RSI:%x\n", regs->rax,
         regs->rbx, regs->rcx, regs->rdx, regs->rsp, regs->rdi, regs->rsi);
  printf("R8:%x R9:%x R10:%x R11:%x R12:%x R13:%x R14:%x R15:%x\n", regs->r8,
         regs->r9, regs->r10, regs->r11, regs->r12, regs->r13, regs->r14,
         regs->r15);
  printf("RIP:%x CS:%x RFLAGS:%x\n", regs->rip, regs->cs, regs->rFlags);
  printf("SS:%x KRSP:%x\n", regs->ss, regs->kRsp);
  panic("");
}

void doPanic(char* err) {
  u32 eax = 1, ebx, ecx, edx;
  cpuid(&eax, &ebx, &ecx, &edx);
  u32 cpuId = (ebx >> 24) & 0xFF;

  if (*err != '\0')
    printf("%s", err);
  printf("CPU: %d\n", cpuId);
  printf("--- Kernel Call Trace ---\n");

  struct Stacktrace* stk;
  asm("movq %%rbp,%0" : "=r"(stk)::);

  for (u64 fr = 0; stk && fr < 10; ++fr) {
    if (stk->rip == 0)
      break;
    printf("%llx\n", stk->rip);
    stk = stk->rbp;
  }

  asm("cli");
  asm("hlt");
}

void panic(char* fmt, ...) {
  va_list va;
  va_start(va);

  char buf[1024];
  memset(buf, 0, 1024);
  vsnprintf(buf, 1024, fmt, va);

  doPanic(buf);

  va_end(va);
}