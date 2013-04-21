#include "queue.h"

/**
 * queue_alloc - dynamically allocate a new queue.
 * @queue:  pointer to the queue.
 * @size:   the number of elements in the queue.
 * @esize:  size of one element in bytes.
 *
 * Return 0 if no error, otherwise an error code.
 */
int queue_alloc(struct queue *queue, unsigned int size, unsigned int esize) {
    queue->in = 0;
    queue->out = 0;
    queue->esize = esize;



    return 0;
}

/**
 * queue_free - free the queue.
 * @queue:  pointer to the queue to be freed.
 */
void queue_free(struct queue *queue) {

    queue->in = 0;
    queue->out = 0;
    queue->esize = 0;
    queue->data = NULL;
}

/**
 * queue_init - initialize a new queue.
 * @queue:  pointer to the queue.
 * @buffer: pointer to the data.
 * @size:   the number of elements in the queue.
 * @esize:  size of one element in bytes.
 *
 * Return 0 if no error, otherwise an error code.
 */
int queue_init(struct queue *queue, void *buffer, unsigned int size,
        unsigned int esize) {
    return -1;
}
