[bits 32]
global _start
extern kernel_main   ; Tells the linker to find kernel_main() in kernel.c

_start:
    ; The bootloader sets up the stack, but let's re-verify it safely here
    mov ebp, 0x90000
    mov esp, ebp

    call kernel_main ; Call your actual C entry point
    jmp $            ; Infinite loop if kernel_main ever returns