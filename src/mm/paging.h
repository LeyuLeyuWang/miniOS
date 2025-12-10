#ifndef MINI_PAGING_H
#define MINI_PAGING_H

#include <stdint.h>

void paging_init(void);
void switch_page_directory(uint32_t *dir);

#endif // MINI_PAGING_H
