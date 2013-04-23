#ifndef MM_H
#define MM_H

#include "common.h"
#include "page.h"

#define MM_1K                   0x0400
#define MM_4K                   0x1000
#define MM_ALIGN_4K             0xFFFFF000

#define PAGE_SIZE               MM_4K

#define INT_INDEX_FROM_BIT(a)   (a / 32)
#define BIT_OFFSET_FROM_BIT(a)  (a % 32)

void init_mm(uint32_t phys_mem_size);
void alloc_frame(page *page, int is_kernel, int is_writable);
void free_frame(page *page);

uint32_t kmalloc_early(uint32_t size, int align, uint32_t *phys);

#endif
