#include "gdt.h"
#include "idt.h"
#include "../drivers/vga.h"
#include "panic.h"

void kernel_main(void)
{
    gdt_init();
    idt_init();

    vga_init();
    vga_clear();
    vga_write("MiniOS v0.1 booting...\n");
    vga_write("Welcome to MiniOS!\n");

    for (;;) {
        __asm__ volatile("hlt");
    }
}
