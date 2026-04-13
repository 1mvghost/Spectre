#include <isr.h>
#include <stdio.h>
#include <idt.h>
#include <mem.h>
extern void isr0(Regs*);
extern void isr1(Regs*);
extern void isr2(Regs*);
extern void isr3(Regs*);
extern void isr4(Regs*);
extern void isr5(Regs*);
extern void isr6(Regs*);
extern void isr7(Regs*);
extern void isr8(Regs*);
extern void isr9(Regs*);
extern void isr10(Regs*);
extern void isr11(Regs*);
extern void isr12(Regs*);
extern void isr13(Regs*);
extern void isr14(Regs*);
extern void isr15(Regs*);
extern void isr16(Regs*);
extern void isr17(Regs*);
extern void isr18(Regs*);
extern void isr19(Regs*);
extern void isr20(Regs*);
extern void isr21(Regs*);
extern void isr22(Regs*);
extern void isr23(Regs*);
extern void isr24(Regs*);
extern void isr25(Regs*);
extern void isr26(Regs*);
extern void isr27(Regs*);
extern void isr28(Regs*);
extern void isr29(Regs*);
extern void isr30(Regs*);
extern void isr31(Regs*);

static void (*isrStub[32])(Regs*) ={
    isr0, 
    isr1, 
    isr2, 
    isr3, 
    isr4, 
    isr5, 
    isr6, 
    isr7,
    isr8, 
    isr9, 
    isr10, 
    isr11, 
    isr12, 
    isr13, 
    isr14, 
    isr15,
    isr16, 
    isr17, 
    isr18, 
    isr19, 
    isr20, 
    isr21, 
    isr22, 
    isr23,
    isr24, 
    isr25, 
    isr26, 
    isr27, 
    isr28, 
    isr29, 
    isr30, 
    isr31
};
static const char *exceptions[32] = {
    "Div By Zero",
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
    "Unknown"
};

Splock splockk = ATOMIC_FLAG_INIT;

void isrHandler(Regs* regs) {
    mSpinlockAcquire(&splockk);
    printf(PANIC,"--- KERNEL PANIC ---\n");
    printf(PANIC,"%s\n", exceptions[regs->intId]);
    printf(PANIC,"STOP:%x INT:%x\n",regs->errId,regs->intId);
    printf(PANIC,"RAX:%x RBX:%x RCX:%x RDX:%x RSP:%x RDI:%x RSI:%x\n", regs->rax, regs->rbx, regs->rcx,regs->rdx,regs->rsp, regs->rdi, regs->rsi);
    printf(PANIC,"R8:%x R9:%x R10:%x R11:%x R12:%x R13:%x R14:%x R15:%x\n",regs->r8,regs->r9,regs->r10,regs->r11,regs->r12,regs->r13,regs->r14,regs->r15);
    printf(PANIC,"RIP:%x CS:%x RFLAGS:%x KRSP: %x:%x\n",regs->rip, regs->cs, regs->rFlags, regs->ss,regs->kRsp);
    mSpinlockDrop(&splockk);
    panic("---\n");
}

void isrInit() {
    for(int i = 0; i < 48; i++) {
        idtSetDesc(i, (void*)isrStub[i], 0x8E);
    }
    asm("sti");
}