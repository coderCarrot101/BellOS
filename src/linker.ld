ENTRY(kernel_main)
OUTPUT_FORMAT("binary")

SECTIONS
{
    /* The kernel will be loaded to 0x9000 by the bootloader */
    . = 0x9000;

    .text : {
        *(.text)
    }

    .rodata : { *(.rodata) }
    .data   : { *(.data) }
    .bss    : { *(.bss) }
}