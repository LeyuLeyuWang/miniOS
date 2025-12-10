#include "vga.h"
#include "stdint.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t vga_color = 0x07; // Light grey on black

static void vga_putentryat(char c, uint8_t color, uint8_t x, uint8_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    VGA_MEMORY[index] = ((uint16_t)color << 8) | (uint8_t)c;
}

void vga_clear(void)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_putentryat(' ', vga_color, x, y);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_init(void)
{
    vga_clear();
}

void vga_putc(char c)
{
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        vga_putentryat(c, vga_color, cursor_x, cursor_y);
        cursor_x++;
    }

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = 0; // TODO: implement scrolling
    }
}

void vga_write(const char *s)
{
    while (*s) {
        vga_putc(*s++);
    }
}
