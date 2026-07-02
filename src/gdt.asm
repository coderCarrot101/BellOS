; Global Descriptor Table (GDT) Setup

gdt_start:
    ; The GDT must always start with a completely null 8-byte descriptor
    dd 0x0
    dd 0x0

gdt_code:
    ; Code Segment Descriptor
    dw 0xffff    ; Limit (bits 0-15) -> 4GB
    dw 0x0       ; Base (bits 0-15)  -> 0x0
    db 0x0       ; Base (bits 16-23)
    db 10011010b ; Access byte (Present, Ring 0, Code, Executable, Readable)
    db 11001111b ; Flags (32-bit granularity) + Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)

gdt_data:
    ; Data Segment Descriptor
    dw 0xffff    ; Limit (bits 0-15) -> 4GB
    dw 0x0       ; Base (bits 0-15)  -> 0x0
    db 0x0       ; Base (bits 16-23)
    db 10010010b ; Access byte (Present, Ring 0, Data, Writable)
    db 11001111b ; Flags (32-bit granularity) + Limit (bits 16-19)
    db 0x0       ; Base (bits 24-31)

gdt_end:

; GDT Descriptor (The pointer the CPU actually loads)
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of our GDT (minus 1)
    dd gdt_start               ; Start address of our GDT

; Constants we will use later to tell the CPU which segment to look at
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start