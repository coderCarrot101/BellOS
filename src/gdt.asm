; Global Descriptor Table

gdt_start:
    ; The GDT must always start with a completely null 8-byte descriptor
    dd 0x0
    dd 0x0

gdt_code:
    ; Code Segment Descriptor
    dw 0xffff    ; Limit 4GB
    dw 0x0
    db 0x0 
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    ; Data Segment Descriptor
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start