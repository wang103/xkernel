#ifndef MM_H
#define MM_H

#include "common.h"
#include "page.h"

#define PHYS_MEM_SIZE_BYTE      0x20000000      // Assume to be 512 MB for now

#define MM_4K                   0x1000
#define MM_ALIGN_4K             0xFFFFF000

#define INT_INDEX_FROM_BIT(a)   (a / 32)
#define BIT_OFFSET_FROM_BIT(a)  (a % 32)

// Defined in mm.s. Takes a 24 byte buffer address as input.
extern void get_phys_mem_size(uint32_t);

void init_mm();
void alloc_frame(page *page, int is_kernel, int is_writable);
void free_frame(page *page);

uint32_t kmalloc_early(uint32_t size, int align, uint32_t *phys);

#endif
