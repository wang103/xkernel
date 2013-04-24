#include "kheap.h"
#include "mm.h"

extern page_directory *kernel_directory;

struct rb_root mem_root;
uint32_t kheap_cur_end = 0;     // 0 as initial value indicates the
                                // heap is not initialized yet.

static void insert_node_into_rbtree(struct rb_node *new_node) {
    struct rb_node **p = &mem_root.rb_node;
    struct rb_node *parent = NULL;

    while (*p) {
        parent = *p;

        if ((uint32_t)(&new_node) < (uint32_t)p) {
            p = &(*p)->left;
        } else {
            p = &(*p)->right;
        }
    }

    rb_link_node(new_node, parent, p);
    rb_insert_fixup(new_node, &mem_root);
}

/**
 * Split the node according to the needed size. If the splitting happens, a new
 * node will be inserted after the provided node.
 */
static void split_node(mem_node *node, uint32_t needed_size) {
    if (node->size > needed_size + sizeof(mem_node)) {
        mem_node *new_node = (mem_node *)(((char *)node) + sizeof(mem_node)
                + needed_size);

        // Update size of both node.
        new_node->size = node->size - needed_size - sizeof(mem_node);
        new_node->in_use = 0;
        node->size = needed_size;

        // Insert the new node into the rb tree.
        insert_node_into_rbtree(&new_node->node);
    }
}

static mem_node *_find_available_node(struct rb_node *node, uint32_t size,
        int align) {
    if (node == NULL) {
        return NULL;
    }

    mem_node *cur_mem_node = rb_entry(node, mem_node, node);

    if (cur_mem_node->in_use == 0 && align) {
        uint32_t location = (uint32_t)cur_mem_node;

        if ((location + sizeof(mem_node)) & MM_ALIGN_4K) {
            uint32_t offset = PAGE_SIZE -
                (location + sizeof(mem_node)) % PAGE_SIZE;
            
            int real_size = cur_mem_node->size - offset;

            if (real_size >= 0 &&
                (uint32_t)real_size >= size &&
                offset >= sizeof(mem_node)) {

                mem_node *new_mem_node = (mem_node *)
                    ((char *)cur_mem_node + offset);

                // Update the sizes.
                new_mem_node->size = real_size;
                cur_mem_node->size -= (real_size + sizeof(mem_node));

                // Add the new node to the rb tree.
                insert_node_into_rbtree(&new_mem_node->node);

                return new_mem_node;
            }

            real_size -= PAGE_SIZE;
            if (real_size >= 0 && (uint32_t)real_size >= size) {
               
                mem_node *new_mem_node = (mem_node *)
                    ((char *)cur_mem_node + offset+ PAGE_SIZE);

                // Update the sizes.
                new_mem_node->size = real_size;
                cur_mem_node->size -= (real_size + sizeof(mem_node));

                // Add the new node to the rb tree.
                insert_node_into_rbtree(&new_mem_node->node);

                return new_mem_node;
            }
        }
        else if (cur_mem_node->size >= size) {
            return cur_mem_node;
        }
    }
    else if (cur_mem_node->in_use == 0 && cur_mem_node->size >= size) {
        return cur_mem_node;
    }

    // Recursive cases:
    mem_node *temp = _find_available_node(node->left, size, align);
    if (temp != NULL) {
        return temp;
    }

    return _find_available_node(node->right, size, align);
}

static mem_node *find_available_node(uint32_t size, int align) {

    mem_node *node = _find_available_node(mem_root.rb_node, size, align);

    if (node == NULL) {
        // Need to expand the boundary of heap for more memory.
        
        uint32_t i = kheap_cur_end;
        uint32_t new_node_end = i + sizeof(mem_node);

        if (align && (new_node_end & MM_ALIGN_4K)) {
            uint32_t offset = PAGE_SIZE - new_node_end % PAGE_SIZE;
            
            struct rb_node *last_rb_node = maximum(mem_root.rb_node);
            if (last_rb_node == NULL) {
                // TODO: handle this case.
            }
            mem_node *last_mem_node = rb_entry(last_rb_node, mem_node, node);
            
            last_mem_node->size += offset;
            kheap_cur_end += offset;

            while (i < kheap_cur_end) {
                page *pg = get_page(i, 1, kernel_directory);
                alloc_frame(pg, 1, 1);

                i += PAGE_SIZE;
            }

            i = kheap_cur_end;
        }

        uint32_t new_bound = i + sizeof(mem_node) + size;

        while (i < new_bound) {
            page *pg = get_page(i, 1, kernel_directory);
            alloc_frame(pg, 1, 1);

            i += PAGE_SIZE;
        }

        node = (mem_node *)kheap_cur_end;
        kheap_cur_end = new_bound;

        node->size = size;

        // Insert the new node into the rb tree.
        insert_node_into_rbtree(&node->node);
    }

    split_node(node, size);

    return node;
}

void init_kheap() {
    mem_root = RB_ROOT;
    kheap_cur_end = KHEAP_START;
}

void *alloc(uint32_t size, int align) {
    mem_node *node = find_available_node(size, align);
    node->in_use = 1;

    return (char *)node + sizeof(mem_node);
}

void free(void *addr) {
    //TODO: implementation.
}
