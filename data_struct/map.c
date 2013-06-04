#include "map.h"
#include "../kernel/kheap.h"

/**
 * map_init - initialize a map.
 * @m:  the map to be initialized.
 */
void map_init(struct map *m) {
    m->map_root = RB_ROOT; 
}

/**
 * add - add a new mapping.
 * If the key already existed, its value will be overwritten.
 * @m:      the map to add to.
 * @ptr:    the value.
 * @id:     the key.
 *
 * Return 0 if no error, otherwise an error code.
 */
int add(struct map *m, void *ptr, int id) {
    map_node *new_m_node = (map_node *)alloc(sizeof(map_node), 0);
    new_m_node->id = id;
    new_m_node->ptr = ptr;

    // Insert the new node into the tree.
    struct rb_node **p = &(m->map_root.rb_node);
    struct rb_node *parent = NULL;
    map_node *m_node;

    while (*p) {
        parent = *p;
        m_node = rb_entry(parent, map_node, node);

        if (id < m_node->id) {
            p = &((*p)->left);
        } else if (id > m_node->id) {
            p = &((*p)->right);
        } else {
            // Simply overwrite the value.
            m_node->ptr = ptr;
            free(new_m_node);
            return 0;
        }
    }

    rb_link_node(&(new_m_node->node), parent, p);
    rb_insert_fixup(&(new_m_node->node), &(m->map_root));

    return 0;
}

/**
 * allocate - add a new mapping, also generate a key for the user.
 * @m:      the map to add to.
 * @ptr:    the value.
 * @id:     the pointer to store the new key at.
 *
 * Return 0 if no error, otherwise an error code.
 */
int allocate(struct map *m, void *ptr, int *id) {
    int uid = 0;

    // Find an available UID by first getting the minimum, and keep checking
    // current node's successor. If the difference between two consecutive
    // nodes are greater than 1, then we can use a number in-between as key.
    struct rb_node *cur_node = minimum(m->map_root.rb_node);
    struct rb_node *next_node;

    if (cur_node != NULL) {
        next_node = successor(cur_node);

        map_node *cur_m_node = rb_entry(cur_node, map_node, node);
        map_node *next_m_node = NULL;
        
        while (next_node != NULL) {
            next_m_node = rb_entry(next_node, map_node, node);

            if ((next_m_node->id - cur_m_node->id) > 1) {
                uid = cur_m_node->id + 1;
                break;
            }

            cur_node = next_node;
            cur_m_node = rb_entry(cur_node, map_node, node);
            next_node = successor(cur_node);
        }

        if (next_node == NULL) {
            uid = cur_m_node->id + 1;
        }
    }

    *id = uid;

    return add(m, ptr, uid);
}

void remove() {

}

int lookup() {
    return -1;
}
