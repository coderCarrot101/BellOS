[org 0x7c00]

mov [BOOT_DRIVE], dl
; 1. 16-Bit Real Mode Initialization
mov si, real_mode_msg
call print_string_16

; --- NEW: LOAD KERNEL FROM DISK INTO RAM ---
mov bx, 0x9000              ; We want to load the kernel to memory address 0x9000
mov ah, 0x02                ; BIOS read sector function
mov al, 2                   ; Read 2 sectors (enough space for our C kernel)
mov ch, 0                   ; Cylinder 0
mov dh, 0                   ; Head 0
mov cl, 2                   ; Start reading from Sector 2 (Sector 1 is our bootloader)
mov dl, [BOOT_DRIVE]        ; Read from our boot drive
int 0x13                    ; Trigger BIOS Disk Interrupt

jc disk_error               ; If carry flag is set, disk read failed!
; -------------------------------------------

; 2. Prep for 32-bit Switch
cli                         ; Clear Interrupts!
lgdt [gdt_descriptor]       ; Load GDT

; 3. Flip the Bit
mov eax, cr0
or eax, 0x1
mov cr0, eax

; 4. Far Jump to Flush Pipeline
jmp CODE_SEG:init_pm

disk_error:
    mov si, disk_err_msg
    call print_string_16
    jmp $

; --- 16-Bit Printing Function ---
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

; --- Include our GDT file ---
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

    ; Jump directly to where our kernel was loaded in memory
    jmp 0x9000

    jmp $

; --- DATA SECTION ---
BOOT_DRIVE db 0
real_mode_msg db "Booting in 16-bit Real Mode...", 13, 10, 0
disk_err_msg  db "Disk Read Error!", 13, 10, 0

; Force the bootloader component to be exactly 512 bytes
times 510 - ($ - $$) db 0
dw 0xaa55