ENTRY(kernel_main)

SECTIONS
{
    KERNEL_VMA = 0x00010000;
    KERNEL_LMA = 0x00000000;

    . = KERNEL_VMA;

    .text : AT(KERNEL_LMA) {
        *(.text*)
        *(.rodata*)
        *(.eh_frame)
    }

    .data : AT(LOADADDR(.text) + SIZEOF(.text)) {
        *(.data*)
        *(.got*)
        *(.got.plt)
    }

    .bss : AT(LOADADDR(.data) + SIZEOF(.data)) {
        *(COMMON)
        *(.bss*)
    }
}
