#ifndef MM_H
#define MM_H

#include "common.h"

#define MM_4K           0x1000
#define MM_ALIGN_4K     0xFFFFF000

uint32_t kmalloc_early(uint32_t size, int align, uint32_t *phys);

#endif
