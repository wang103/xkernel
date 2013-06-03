#ifndef MAP_H
#define MAP_H

#include "../kernel/common.h"
#include "rbtree.h"

/**
 * Implementation of associative array.
 * This is implemented using a self-balancing binary tree.
 *
 * The mapping is from a unique identification number (UID) to a pointer.
 */

struct map {
    struct rb_root map_root;
};

void map_init(struct map *m);

void add();
void allocate();
void remove();
int lookup();

#endif
