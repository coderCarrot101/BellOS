[org 0x7c00]

jmp 0x0000:start_boot

start_boot:
    xor ax, ax 
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    mov [BOOT_DRIVE], dl

    ;PRINT INITIALIZATION MESSAGE
    mov si, real_mode_msg
    call print_string_16

    ;LOAD KERNEL
    mov bx, 0x9000
    mov ah, 0x02
    mov al, 32          ; Read 32 sectors (~16KB)
    mov ch, 0
    mov dh, 0
    mov cl, 2
    mov dl, [BOOT_DRIVE]
    int 0x13 

    jc disk_error       ; carry flag handle error

    cli 
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:init_pm

disk_error:
    cli
    hlt
    jmp $

print_string_16:
    mov ah, 0x0e
.loop:
    mov al, [si]
    inc si
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

%include "src/gdt.asm"

; --- 32-Bit Protected Mode Entry Point ---
[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    jmp 0x9000

    jmp $

BOOT_DRIVE db 0
real_mode_msg db "Booting in 16-bit Real Mode...", 13, 10, 0
disk_err_msg  db "Disk Read Error!", 13, 10, 0

; Force the bootloader component to be exactly 512 bytes
times 510 - ($ - $$) db 0
dw 0xaa55