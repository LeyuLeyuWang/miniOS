#ifndef MINI_STRING_H
#define MINI_STRING_H

#include "stdint.h"

void *memset(void *dest, int c, size_t len);
void *memcpy(void *dest, const void *src, size_t len);
size_t strlen(const char *str);
int strcmp(const char *a, const char *b);

#endif /* MINI_STRING_H */
