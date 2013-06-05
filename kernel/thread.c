#include "thread.h"
#include "kheap.h"

// Declare a list head for a list of threads.
LIST_HEAD(thread_list);

// Current executing thread.
thread_t *current_thread = NULL;
static uint32_t next_id = 0;

/**
 * init_threading - initialize multithread.
 */
thread_t *init_threading() {
    // This is the current running thread, aka the idle thread. Therefore we
    // don't ever need to free its memory.
    thread_t *thread = alloc(sizeof(thread_t), 0);
    thread->id = next_id++;

    current_thread = thread;

    return thread;
}

/**
 * create_thread - create a new thread.
 * Newly created thread will be the next to run after the current thread.
 *
 * @fn:     function pointer for this thread to run.
 * @arg:    pointer to parameter.
 * @stack:  stack pointer for this thread.
 */
thread_t *create_thread(int (*fn)(void *), void *arg, uint32_t *stack) {
    return NULL;
}

/**
 * switch_thread - switch to next thread to run.
 * Should be called periodically by the timer.
 */
void switch_thread() {

}
