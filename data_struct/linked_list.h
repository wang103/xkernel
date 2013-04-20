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
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

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
 * list_add_tail - add a new entry.
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

/**
 * list_del - delete entry from list.
 * @entry:  the entry to delete from the list.
 */
static inline void list_del(struct list_head *entry) {
    struct list_head *prev = entry->prev;
    struct list_head *next = entry->next;

    prev->next = next;
    next->prev = prev;

    entry->prev = NULL;
    entry->next = NULL;
}

/**
 * list_del_init - delete entry from list and reinitialize it.
 * @entry:  the entry to delete from the list.
 */
static inline void list_del_init(struct list_head *entry) {
    list_del(entry);
    INIT_LIST_HEAD(entry);
}

/**
 * list_move - delete from one list and add after another's head.
 * @entry:  the entry to move.
 * @head:   the head that will precede the entry.
 */
static inline void list_move(struct list_head *entry, struct list_head *head) {
    list_del(entry);
    list_add(entry, head);
}

/**
 * list_move_tail - delete from one list and add it as another's tail.
 * @entry:  the entry to move.
 * @head:   the head that will follow the entry.
 */
static inline void list_move_tail(struct list_head *entry,
        struct list_head *head) {
    list_del(entry);
    list_add_tail(entry, head);
}

/**
 * list_empty - test whether or not a list is empty.
 * @head:   the list to test.
 */
static inline int list_empty(const struct list_head *head) {
    return head->next == head;
}

static inline void __list_splice(const struct list_head *list,
        struct list_head *prev, struct list_head *next) {
    struct list_head *first = list->next;
    struct list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

/**
 * list_splice - join two lists.
 * @list:   the list to add.
 * @head:   the place to add the first list.
 */
static inline void list_splice(struct list_head *list, struct list_head *head) {
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
    }
}

/**
 * list_splice_tail - join two lists.
 * @list:   the list to add.
 * @head:   the place to add the first list.
 */
static inline void list_splice_tail(struct list_head *list,
        struct list_head *head) {
    if (!list_empty(list)) {
        __list_splice(list, head->prev, head);
    }
}

/**
 * list_splice_init - join two lists and reinitialize the emptied list.
 * @list:   the list to add.
 * @head:   the place to add the first list.
 */
static inline void list_splice_init(struct list_head *list,
        struct list_head *head) {
    if (!list_empty(list)) {
        __list_splice(list, head, head->next);
        INIT_LIST_HEAD(list);
    }
}

/**
 * list_splice_tail_init - join two lists and reinitialize the emptied list.
 * @list:   the list to add.
 * @head:   the place to add the first list.
 */
static inline void list_splice_tail_init(struct list_head *list,
        struct list_head *head) {
    if (!list_empty(list)) {
        __list_splice(list, head->prev, head);
        INIT_LIST_HEAD(list);
    }
}

/**
 * list_for_each - iterate over a list.
 * @pos:    the &struct list_head to use a loop cursor.
 * @head:   the head of the list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_entry - iterate over list of give type.
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head of the list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)                      \
    for (pos = list_entry((head)->next, typeof(*pos), member);      \
         &pos->member != (head);                                    \
         pos = list_entry(pos->member.next, typeof(*pos), member))

#endif
