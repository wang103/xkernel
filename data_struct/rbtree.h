#ifndef RB_TREE_H
#define RB_TREE_H

#include "../kernel/common.h"

/**
 * Implementation of red-black tree.
 *
 * Rules for semi-balanced (depth difference is no more than double):
 * 1. All nodes are either red or black.
 * 2. Leaf nodes are black.
 * 3. Leaf nodes do not contain data.
 * 4. All non-leaf nodes have two children.
 * 5. If a node is red, both of its children are black.
 * 6. The path from a node to one of its leaves contains the same number of
 * black nodes as the the shortest path to any of its other leaves.
 */

struct rb_node {
    struct rb_node *parent;
    struct rb_node *left;
    struct rb_node *right;

    uint8_t is_red;
};

struct rb_root {
    struct rb_node *rb_node;
};

// Declare a rb tree root.
#define RB_ROOT (struct rb_root) { NULL }

/**
 * rb_entry - get the struct for this entry.
 * @ptr:    the &struct rb_node pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the rb_node within the struct.
 */
#define rb_entry(ptr, type, member) container_of(ptr, type, member)

#define RB_EMPTY_ROOT(root) ((root)->rb_node == NULL)

/**
 * Add node as child of parent. *rb_link will be set to node.
 */
static inline void rb_link_node(struct rb_node *node, struct rb_node *parent,
        struct rb_node **rb_link) {
    node->parent = parent;
    node->rb_left = NULL;
    node->rb_right = NULL;

    node->is_red = 1;

    *rb_link = node;
}

struct rb_node *minimum(struct rb_node *);
struct rb_node *maximum(struct rb_node *);
struct rb_node *predecessor(struct rb_node *);
struct rb_node *successor(struct rb_node *);

void rb_insert_fixup(struct rb_node *, struct rb_root *);
void rb_erase_fixup(struct rb_node *, struct rb_root *);

#endif
