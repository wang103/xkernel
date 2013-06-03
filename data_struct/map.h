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

typedef struct _map_node {
    int id;
    void *ptr;
    struct rb_node node;
} map_node;

struct map {
    struct rb_root map_root;
};

void map_init(struct map *m);

int add(struct map *m, void *ptr, int id);
int allocate(struct map *m, void *ptr, int *id);
void remove();
int lookup();

#endif
