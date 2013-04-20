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

/**
 * queue_alloc - dynamically allocate a new queue.
 * @que:    pointer to the queue.
 * @size:   the number of elements in the queue, this must be a power of 2.
 *
 * The number of elements will be rounded-up to a power of 2.
 * Return 0 if no error, otherwise an error code.
 */
#define queue_alloc(que, size) {                \
    typeof((que) + 1) __tmp = (que);            \


/**
 * queue_free - free the queue.
 * @que:    pointer to the queue to be freed.
 */
#define queue_free(que)

static void queue_init(struct queue *que, void *buffer, unsigned int size) {

}

#define DECLARE_QUEUE(name, size)

#define INIT_QUEUE(name)

static enqueue() {
}

static dequeue() {
}

#endif
