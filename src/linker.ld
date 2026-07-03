ENTRY(_start)

SECTIONS
{
    /* Must match your bootloader's jump address exactly */
    . = 0x9000;

    .text : {
        /* Force the entry object file's code to be placed FIRST */
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