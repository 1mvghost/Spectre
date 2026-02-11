[BITS 64]
[EXTERN isrHandler]
[SECTION .text]
%macro ISR_ERR 1
global isr%+%1
isr%+%1:
    push %1
    jmp isrCommon
%endmacro
%macro ISR_NO_ERR 1
global isr%+%1
isr%+%1:
    push 0
    push %1
    jmp isrCommon
%endmacro

ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR    8
ISR_NO_ERR 9
ISR_ERR    10
ISR_ERR    11
ISR_ERR    12
ISR_ERR    13
ISR_ERR    14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_ERR    17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_ERR    30
ISR_NO_ERR 31
isrCommon:
    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push rsp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    push rdi
    mov  rdi, rsp
    call isrHandler
    pop  rdi

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsp
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16
    iretq