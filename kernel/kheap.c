#include "kheap.h"
#include "mm.h"

static void *split_node(mem_node *node, uint32_t needed_size) {
    
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
        node = _find_available_node(mem_root.rb_node, sizeof(mem_node));

        if (node == NULL) {
            node = kmalloc_early(sizeof(mem_node) + size, 0, NULL);
            node->mem_addr = (uint32_t)(((char *)node) + sizeof(mem_node));
            node->size = size;
        }
        else {
            // Could find slot for requested memory, but find slot for the
            // node structure.
            split_node(node, sizeof(mem_node));


        }
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

    return (void *)node->mem_addr;
}

void free(void *addr) {

}
