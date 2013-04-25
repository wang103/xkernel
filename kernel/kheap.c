#include "kheap.h"
#include "mm.h"

extern page_directory *kernel_directory;

uint32_t kheap_end = 0;         // The total size for heap will be 1/8
                                // of the total RAM.

struct rb_root mem_root;
uint32_t kheap_cur_end = 0;     // 0 as initial value indicates the
                                // heap is not initialized yet.

static void insert_node_into_rbtree(struct rb_node *new_node) {
    struct rb_node **p = &mem_root.rb_node;
    struct rb_node *parent = NULL;

    while (*p) {
        parent = *p;

        if ((uint32_t)new_node < (uint32_t)(*p)) {
            p = &(*p)->left;
        } else if ((uint32_t)new_node > (uint32_t)(*p)) {
            p = &(*p)->right;
        } else {
            PANIC("Node already exists");
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

        if ((location + sizeof(mem_node)) & (~MM_ALIGN_4K)) {
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
        
        uint32_t node_size = size + (align ? PAGE_SIZE : 0);
        uint32_t expand_size = sizeof(mem_node) + node_size;

        uint32_t i = kheap_cur_end;
        uint32_t new_end = kheap_cur_end + expand_size;

        if (new_end >= kheap_end) {
            PANIC("KHEAP OUT OF MEMORY");
        }

        while (i < new_end) {
            page *pg = get_page(i, 1, kernel_directory);
            alloc_frame(pg, 1, 1);

            i += PAGE_SIZE;
        }

        node = (mem_node *)kheap_cur_end;
        node->size = node_size;
        kheap_cur_end = new_end;

        // Insert the new node into the rb tree.
        insert_node_into_rbtree(&node->node);

        // Do another find, this time will find one that fits for sure.
        node = _find_available_node(&node->node, size, align);
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
    struct rb_node *n = mem_root.rb_node;
    uint32_t target_addr = (uint32_t)addr;

    while (n) {
        mem_node *cur_node = rb_entry(n, mem_node, node);
        uint32_t cur_node_addr = (uint32_t)((char *)cur_node +
                sizeof(mem_node));

        if (target_addr < cur_node_addr) {
            n = n->left;
        } else if (target_addr > cur_node_addr) {
            n = n->right;
        } else {
            if (cur_node->in_use == 0) {
                PANIC("Double free");
            }
            cur_node->in_use = 0;

            // Try merging with right.
            struct rb_node *right_n = successor(n);
            if (right_n) {
                mem_node *right_node = rb_entry(right_n, mem_node, node);

                if (right_node->in_use == 0) {
                    cur_node->size += (sizeof(mem_node) + right_node->size);

                    // Remove right node.
                    rb_erase(right_n, &mem_root);
                }
            }

            // Try merging with left.
            struct rb_node *left_n = predecessor(n);
            if (left_n) {
                mem_node *left_node = rb_entry(left_n, mem_node, node);
                
                if (left_node->in_use == 0) {
                    left_node->size += (sizeof(mem_node) + cur_node->size);

                    // Remove node n.
                    rb_erase(n, &mem_root);
                }
            }

            return;
        }
    }

    // Address not found.
    PANIC("Address not allocated before");
}
