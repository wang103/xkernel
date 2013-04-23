/**
 * Dynamic kernel memory allocation.
 * Assume paging has been enabled.
 */

#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"
#include "../data_struct/rbtree.h"

/**
 * |==========|===========|
 * | mem_node | allocated | ... ...
 * | struct   | memory    | ... ...
 * |==========|===========|
 */

typedef struct _mem_node {
    uint32_t size;
    uint8_t in_use;

    struct rb_node node;
} mem_node;

static struct rb_root mem_root;
static uint32_t kheap_cur_end;

#define KHEAP_START     0xC0000000      // Place the kernel-heap at 3G, this
                                        // gives user-space about 3G

void init_kheap();
void *alloc(uint32_t size);
void free(void *addr);

#endif
