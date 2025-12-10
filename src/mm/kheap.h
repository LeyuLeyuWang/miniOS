#ifndef MINI_KHEAP_H
#define MINI_KHEAP_H

#include <stdint.h>

void kheap_init(void);
void *kmalloc(uint32_t size);
void kfree(void *ptr);

#endif // MINI_KHEAP_H
