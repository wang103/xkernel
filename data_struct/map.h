#ifndef MAP_H
#define MAP_H

#include "../kernel/common.h"

/**
 * Implementation of associative array.
 * This is implemented using a self-balancing binary tree.
 *
 * The mapping is from a unique identification number (UID) to a pointer.
 */

void add();
void allocate();
void remove();
int lookup();

#endif
