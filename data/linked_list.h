#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "../kernel/common.h"

/**
 * Implementation of circular doubly linked list.
 */

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

// For compile time static initialization.
#define LIST_HEAD_INIT(name) {&(name), &(name) }

// For run time dynamic initialization.
static inline void INIT_LIST_HEAD(struct list_head *list) {
    list->next = list;
    list->prev = list;
}

// Declare a list head.
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

/**
 * list_entry - get the struct for this entry
 * @ptr:        the &struct list_head pointer.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

#endif
