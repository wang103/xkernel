#include "kheap.h"
#include "mm.h"

static void split_node(mem_node *node, uint32_t needed_size) {
    if (node->size > needed_size + sizeof(mem_node)) {
        mem_node *new_node = (mem_node *)(((char *)node) + sizeof(mem_node)
                + needed_size);

        // Update size of both node.
        new_node->size = node->size - needed_size - sizeof(mem_node);
        new_node->in_use = 0;
        node->size = needed_size;

        // Insert the new node into the rb tree.
        struct rb_node **p = &mem_root.rb_node;
        struct rb_node *parent = NULL;

        while (*p) {
            parent = *p;

            if ((uint32_t)(&new_node->node) < (uint32_t)(*p)) {
                p = &(*p)->left;
            } else {
                p = &(*p)->right;
            }
        }

        rb_link_node(&new_node->node, parent, p);
        rb_insert_fixup(&new_node->node, &mem_root);
    }
}

static mem_node *_find_available_node(struct rb_node *node, uint32_t size) {
    if (node == NULL) {
        return NULL;
    }

    mem_node *cur_mem_node = rb_entry(node, mem_node, node);

    if (cur_mem_node->size >= size && cur_mem_node->in_use == 0) {
        return cur_mem_node;
    }

    mem_node *temp = _find_available_node(node->left, size);
    if (temp != NULL) {
        return temp;
    }

    return _find_available_node(node->right, size);
}

static mem_node *find_available_node(uint32_t size) {

    mem_node *node = _find_available_node(mem_root.rb_node, size);

    if (node == NULL) {
        void *temp_mem = (void *)kmalloc_early(sizeof(mem_node) + size,
                0, NULL);
        node = (mem_node *)temp_mem;
        node->size = size;
    }

    split_node(node, size);

    return node;
}

void init_kheap() {
    mem_root = RB_ROOT;
}

void *kmalloc(uint32_t size) {
    mem_node *node = find_available_node(size);
    node->in_use = 1;

    return (char *)node + sizeof(mem_node);
}

void free(void *addr) {

}
