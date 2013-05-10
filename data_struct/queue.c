#include "queue.h"
#include "../kernel/kheap.h"

/**
 * queue_alloc - dynamically allocate a new queue.
 * @queue:  pointer to the queue.
 * @size:   the number of elements in the queue.
 * @esize:  the size of each element.
 *
 * Return 0 if no error, otherwise an error code.
 */
int queue_alloc(struct queue *queue, unsigned int size,
        unsigned int esize) {
    queue->in = 0;
    queue->out = 0;
    queue->total_size = size * esize;
    queue->is_full = 0;

    queue->data = alloc(size * esize, 0);

    return 0;
}

/**
 * queue_free - free the queue.
 * @queue:  pointer to the queue to be freed.
 */
void queue_free(struct queue *queue) {
    queue->in = 0;
    queue->out = 0;
    queue->total_size = 0;
    queue->is_full = 0;

    free(queue->data);
    queue->data = NULL;
}

/**
 * queue_init - initialize a new queue.
 * @queue:  pointer to the queue.
 * @buffer: pointer to the data.
 * @size:   the size of buffer in bytes.
 *
 * Return 0 if no error, otherwise an error code.
 */
int queue_init(struct queue *queue, void *buffer, unsigned int size) {
    queue->in = 0;
    queue->out = 0;
    queue->total_size = size;
    queue->is_full = 0;

    queue->data = buffer;

    return 0;
}

/**
 * queue_in - copy len bytes starting at from into the queue. If less than len
 * bytes are free in the queue, the function copies only up to the amount of
 * available bytes.
 * @queue:  pointer to the queue.
 * @from:   the pointer to copy from.
 * @len:    number of bytes to copy.
 *
 * Return the number of bytes enqueued.
 */
unsigned int queue_in(struct queue *queue, const void *from, unsigned int len) {
    unsigned int count;

    for (count = 0; count < len; count++) {
        if (queue->is_full) {
            break;
        }

        ((char *)(queue->data))[queue->in++] = ((char *)from)[count];

        if (queue->in == queue->total_size) {
            queue->in = 0;
        }

        if (queue->in == queue->out) {
            // Queue is full.
            queue->is_full = 1;
            break;
        }
    }

    return count;
}

/**
 * queue_out - copy len bytes from the queue to 'to'. If less than len bytes are
 * available in the queue, the function copies only up to the amount of
 * available bytes.
 * @queue:  pointer to the queue.
 * @to:     the pointer to copy to.
 * @len:    number of bytes to copy.
 *
 * Return the number of bytes dequeued.
 */
unsigned int queue_out(struct queue *queue, void *to, unsigned int len) {
    unsigned int count;

    for (count = 0; count < len; count++) {
        if (queue->out == queue->in) {
            // Queue is empty.
            break;
        }

        ((char *)to)[count] = ((char *)(queue->data))[queue->out];

        queue->out++;

        if (queue->out == queue->total_size) {
            queue->out = 0;
        }
    }

    queue->is_full = 0;

    return count;
}

/**
 * queue_out_peek - peek the data in the queue without removing them.
 * @queue:  pointer to the queue.
 * @to:     the pointer to copy to.
 * @len:    number of bytes to copy.
 * @offset: offset into the queue to peek at (in bytes).
 *
 * Return the number of bytes peeked.
 */
unsigned int queue_out_peek(struct queue *queue, void *to, unsigned int len,
        unsigned offset) {
    unsigned int count;
    unsigned int temp_out = (queue->out + offset) % queue->total_size;

    for (count = 0; count < len; count++) {
        if (temp_out == queue->in) {
            // Queue is empty.
            break;
        }

        ((char *)to)[count] = ((char *)(queue->data))[temp_out];

        temp_out++;

        if (temp_out == queue->total_size) {
            temp_out = 0;
        }
    }

    return count;
}

unsigned int queue_size(struct queue *queue) {
    return queue->total_size;
}

unsigned int queue_enqueued_size(struct queue *queue) {
    return queue->in - queue->out;
}

unsigned int queue_avail_size(struct queue *queue) {
    return queue->total_size - queue->in + queue->out;
}

int queue_is_empty(struct queue *queue) {
    return (queue->in == queue->out) && (!queue->is_full);
}

int queue_is_full(struct queue *queue) {
    return queue->is_full;
}

void queue_reset(struct queue *queue) {
    queue->in = 0;
    queue->out = 0;
    queue->is_full = 0;
}
