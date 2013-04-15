#ifndef MM_H
#define MM_H

#include "common.h"
#include "page.h"

#define MM_4K           0x1000
#define MM_ALIGN_4K     0xFFFFF000

#define BYTE_INDEX_FROM_BIT(a)  (a / 32)
#define BIT_OFFSET_FROM_BIT(a)  (a % 32)

void alloc_frame(page *page, int is_kernel, int is_writable);
void free_frame(page *page);

uint32_t kmalloc_early(uint32_t size, int align, uint32_t *phys);

#endif
