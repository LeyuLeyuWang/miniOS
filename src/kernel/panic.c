#include "../drivers/vga.h"
#include "panic.h"

void panic(const char *msg)
{
    vga_write("Kernel panic: ");
    vga_write(msg);
    vga_write("\nSystem halted.\n");

    for (;;) {
        __asm__ volatile("hlt");
    }
}
