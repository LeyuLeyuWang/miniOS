#include "string.h"

void *memset(void *dest, int c, size_t len)
{
    unsigned char *d = dest;
    while (len--) {
        *d++ = (unsigned char)c;
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t len)
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (len--) {
        *d++ = *s++;
    }
    return dest;
}

size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

int strcmp(const char *a, const char *b)
{
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(const unsigned char *)a - *(const unsigned char *)b;
}
