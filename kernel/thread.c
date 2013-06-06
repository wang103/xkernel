#include "thread.h"
#include "kheap.h"
#include "common.h"

// Declare a list head for a list of threads.
LIST_HEAD(thread_list);

// Current executing thread.
thread_t *current_thread;
static uint32_t next_id;

/**
 * init_threading - initialize multithread.
 */
void init_threading() {
    current_thread = NULL;
    next_id = 0;
}

/**
 * create_thread - create a new thread.
 *
 * @fn:     function pointer for this thread to run.
 * @arg:    pointer to parameter.
 * @stack:  stack pointer for this thread.
 */
thread_t *create_thread(int (*fn)(void *), void *arg, uint32_t *stack) {
    thread_t *thread = alloc(sizeof(thread_t), 0);
    memset((uint8_t *)thread, 0, sizeof(thread_t));
    thread->id = next_id++;

    *--stack = (uint32_t)arg;
    *--stack = (uint32_t)&exit_cur_thread;
    if (current_thread == NULL) {
        *--stack = 0;   // Fake EBP for the idle process.
    } else {
        *--stack = current_thread->ebp;     // Old EBP.
    }

    thread->esp = (uint32_t)stack;
    thread->ebp = (uint32_t)stack;

    // Add it to the list.
    list_add(&thread->list, &thread_list);

    return thread;
}

/**
 * exit_cur_thread - exit the current running thread.
 * Will start the next thread.
 */
void exit_cur_thread() {
    // Remove it from the list.
    //list_del(&current_thread->list);

    // Free the memory.
    //free(current_thread);

    //switch_thread();
}

/**
 * exit_thread - exit a particular thread.
 *
 * @thread:     the thread to exit.
 */
void exit_thread(thread_t *thread) {

}

/**
 * switch_thread - switch to next thread to run.
 * Should be called periodically by the timer, but also could be triggered due
 * to other reasons, such as task IO or task completion.
 */
void switch_thread(thread_t* thread) {
    // Save current thread's registers.
    asm volatile ("mov %%esp, %0" : "=r" (current_thread->esp));
    asm volatile ("mov %%ebp, %0" : "=r" (current_thread->ebp));
    asm volatile ("mov %%eax, %0" : "=r" (current_thread->eax));
    asm volatile ("mov %%ebx, %0" : "=r" (current_thread->ebx));
    asm volatile ("mov %%ecx, %0" : "=r" (current_thread->ecx));
    asm volatile ("mov %%edx, %0" : "=r" (current_thread->edx));
    asm volatile ("mov %%esi, %0" : "=r" (current_thread->esi));
    asm volatile ("mov %%edi, %0" : "=r" (current_thread->edi));

    // Retrieve next thread's registers.
    asm volatile ("mov %0, %%edi" : : "r" (thread->edi));
    asm volatile ("mov %0, %%esi" : : "r" (thread->esi));
    asm volatile ("mov %0, %%edx" : : "r" (thread->edx));
    asm volatile ("mov %0, %%ecx" : : "r" (thread->ecx));
    asm volatile ("mov %0, %%ebx" : : "r" (thread->ebx));
    asm volatile ("mov %0, %%eax" : : "r" (thread->eax));
    asm volatile ("mov %0, %%ebp" : : "r" (thread->ebp));
    asm volatile ("mov %0, %%esp" : : "r" (thread->esp));
}
