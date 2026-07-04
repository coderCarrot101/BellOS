ENTRY(_start)

SECTIONS
{
    . = 0x9000;

    .text : {
        *kernel_entry.o(.text)
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.common)
        *(.bss)
    }
}