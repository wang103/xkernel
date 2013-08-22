#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "thread.h"

void init_scheduler();
void thread_is_ready(thread_t *thread);
void thread_is_not_ready(thread_t *thread);
void schedule();

#endif
