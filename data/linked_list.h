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
#define LIST_HEAD_INIT(name) { &(name), &(name) }

// For run time dynamic initialization.
static inline void INIT_LIST_HEAD(struct list_head *list) {
    list->next = list;
    list->prev = list;
}

// Declare a list head.
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

/**
 * list_entry - get the struct for this entry.
 * @ptr:        the &struct list_head pointer.
 * @type:       the type of the struct this is embedded in.
 * @member:     the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/**
 * list_add - add a new entry.
 * @new:    new entry to be added.
 * @head:   list head to add it after.
 *
 * Useful for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head) {
    struct list_head *head_next = head->next;
    head_next->prev = new;
    new->next = head_next;
    new->prev = head;
    head->next = new;
}

/**
 * list_add_tail - add a nwe entry.
 * @new:    new entry to be added.
 * @head:   list head to add it before.
 *
 * Useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head) {
    struct list_head *head_prev = head->prev;
    head->prev = new;
    new->next = head;
    new->prev = head_prev;
    head_prev->next = new;
}

#endif
