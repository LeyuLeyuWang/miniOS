#include "kheap.h"

// TODO: Provide a kernel heap allocator (likely a simple bump allocator initially).
void kheap_init(void)
{
}

void *kmalloc(uint32_t size)
{
    (void)size;
    return 0;
}

void kfree(void *ptr)
{
    (void)ptr;
}
