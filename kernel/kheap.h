#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"
#include "../data_struct/rbtree.h"

typedef struct _mem_node {
    uint32_t mem_addr;
    uint32_t size;
    uint8_t in_use;

    struct rb_node node;
} mem_node;

static struct rb_root mem_root;

void init_kheap();
void *kmalloc(uint32_t size);
void free(void *addr);

#endif
