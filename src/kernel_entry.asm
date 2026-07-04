[bits 32]
global _start
extern kernel_main

_start:
    mov ebp, 0x90000
    mov esp, ebp

    call kernel_main
    jmp $