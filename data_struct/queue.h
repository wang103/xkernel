#ifndef QUEUE_H
#define QUEUE_H

#include "../kernel/common.h"

/**
 * Implementation of queue.
 */

struct queue {
    unsigned int    in;
    unsigned int    out;
    unsigned int    esize;
    void            *data;
};

int queue_alloc(struct queue *queue, unsigned int size, unsigned int esize);

void queue_free(struct queue *queue);

int queue_init(struct queue *queue, void *buffer, unsigned int size,
        unsigned int esize);

/**
 * DECLARE_QUEUE - macro to declare a queue object.
 * @name:   name of the declared queue.
 * @type:   type of the queue elements.
 * @size:   number of elements int he queue.
 */
#define DECLARE_QUEUE(name, type, size)     NULL

#define INIT_QUEUE(name)                    NULL

#endif
