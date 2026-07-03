[org 0x7c00]

; 1. FORCE THE BIOS TO ALIGN CS TO 0x0000 VIA A FAR JUMP
jmp 0x0000:start_boot

start_boot:
    ; 2. EXPLICITLY SANITIZE ALL SEGMENT REGISTERS
    xor ax, ax          ; ax = 0
    mov ds, ax          ; Data Segment = 0
    mov es, ax          ; Extra Segment = 0 (Crucial for int 0x13!)
    mov ss, ax          ; Stack Segment = 0
    mov sp, 0x7c00      ; Set a temporary safe 16-bit stack point below the bootloader

    ; 3. NOW SAVE THE BOOT DRIVE SAFE AND SOUND
    mov [BOOT_DRIVE], dl

    ; 4. PRINT INITIALIZATION MESSAGE
    mov si, real_mode_msg
    call print_string_16

    ; 5. LOAD KERNEL FROM DISK INTO RAM
    mov bx, 0x9000      ; Destination offset: 0x9000 (ES is already 0, so 0x0000:0x9000)
    mov ah, 0x02        ; BIOS read sector function
    mov al, 32          ; Read 32 sectors (~16KB)
    mov ch, 0           ; Cylinder 0
    mov dh, 0           ; Head 0
    mov cl, 2           ; Sector 2 (The first sector after the bootloader)
    mov dl, [BOOT_DRIVE]
    int 0x13            ; Fire BIOS interrupt

    jc disk_error       ; If carry flag set, handle error safely

    ; 6. PREP FOR 32-BIT SWITCH (Your remaining code)
    cli 
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:init_pm

disk_error:
    ; Hardware fallback: if the BIOS loop continues crashing on text print,
    ; just raw-halt the CPU without making additional interrupt calls
    cli
    hlt
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