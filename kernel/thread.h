#ifndef THREAD_H
#define THREAD_H

#include "common.h"
#include "../data_struct/linked_list.h"

typedef struct _threat_t {
    uint32_t id;
    uint32_t stack_reg;
    uint32_t base_reg;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;

    struct list_head list;
} thread_t;

// Declare a list head for a list of threads.
LIST_HEAD(thread_list);

// Current executing thread.
thread_t *current_thread = NULL;

thread_t *init_threading();
thread_t *create_thread(int (*fn)(void *), void *arg, uint32_t *stack);
void switch_thread();

#endif
