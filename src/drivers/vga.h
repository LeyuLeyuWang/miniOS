#ifndef MINI_VGA_H
#define MINI_VGA_H

#include <stdint.h>

void vga_init(void);
void vga_clear(void);
void vga_putc(char c);
void vga_write(const char *s);

#endif // MINI_VGA_H
