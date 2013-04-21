#include "queue.h"

/**
 * queue_alloc - dynamically allocate a new queue.
 * @que:    pointer to the queue.
 * @size:   the number of elements in the queue.
 * @esize:  size of one element in bytes.
 *
 * Return 0 if no error, otherwise an error code.
 */
int queue_alloc(struct queue *que, unsigned int size, unsigned int esize) {
    que->in = 0;
    que->out = 0;
    que->esize = esize;



    return 0;
}

/**
 * queue_free - free the queue.
 * @que:    pointer to the queue to be freed.
 */
void queue_free(struct queue *que) {

    que->in = 0;
    que->out = 0;
    que->esize = 0;
    que->data = NULL;
}

/**
 * queue_init - initialize a new queue.
 * @que:    pointer to the queue.
 * @buffer: pointer to the data.
 * @size:   the number of elements in the queue.
 * @esize:  size of one element in bytes.
 *
 * Return 0 if no error, otherwise an error code.
 */
int queue_init(struct queue *que, void *buffer, unsigned int size,
        unsigned int esize) {
    return -1;
}