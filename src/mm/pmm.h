#ifndef MINI_PMM_H
#define MINI_PMM_H

#include <stdint.h>

void pmm_init(uint32_t mem_size);
void *pmm_alloc_block(void);
void pmm_free_block(void *p);

#endif // MINI_PMM_H
