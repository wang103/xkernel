#ifndef THREAD_H
#define THREAD_H

#include "common.h"
#include "../data_struct/linked_list.h"

typedef struct _threat_t {
    uint32_t id;

    uint32_t esp;
    uint32_t ebp;

    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;

    struct list_head list;
} thread_t;

void init_threading();
thread_t *create_thread(int (*fn)(void *), void *arg, uint32_t *stack);
void exit_cur_thread();
void exit_thread(thread_t *thread);
void switch_thread(thread_t *thread);

#endif
