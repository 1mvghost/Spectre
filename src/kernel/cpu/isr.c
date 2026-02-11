#include <isr.h>
#include <stdio.h>
#include <idt.h>
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

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

void isrHandler(Regs* regs) {
    printf(PANIC,"--- KERNEL PANIC ---\n");
    printf(PANIC,"%s\n", exceptions[regs->intId]);
    printf(PANIC,"STOP:%x INT:%x\n",regs->errId,regs->intId);
    printf(PANIC,"RAX:%x RBX:%x RCX:%x RDX:%x RSP:%x RDI:%x RSI:%x\n", regs->rax, regs->rbx, regs->rcx,regs->rdx,regs->rsp, regs->rdi, regs->rsi);
    printf(PANIC,"R8:%x R9:%x R10:%x R11:%x R12:%x R13:%x R14:%x R15:%x\n",regs->r8,regs->r9,regs->r10,regs->r11,regs->r12,regs->r13,regs->r14,regs->r15);
    printf(PANIC,"RIP:%x CS:%x RFLAGS:%x KRSP: %x:%x\n",regs->rip, regs->cs, regs->rFlags, regs->ss,regs->kRsp);
    panic("\n");
    keypress();
}

void isrInit() {
    for(int i = 0; i < 48; i++) {
        idtSetDesc(i, (void*)isrStub[i], 0x8E);
    }
    asm("sti");
}