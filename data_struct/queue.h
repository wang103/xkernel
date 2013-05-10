#ifndef QUEUE_H
#define QUEUE_H

#include "../kernel/common.h"

/**
 * Implementation of queue.
 */

struct queue {
    unsigned int    in;
    unsigned int    out;
    unsigned int    total_size;
    char            is_full;
    void            *data;
};

int queue_alloc(struct queue *queue, unsigned int size, unsigned int esize);

void queue_free(struct queue *queue);

int queue_init(struct queue *queue, void *buffer, unsigned int size);

unsigned int queue_in(struct queue *queue, const void *from, unsigned int len);

unsigned int queue_out(struct queue *queue, void *to, unsigned int len);

unsigned int queue_out_peek(struct queue *queue, void *to, unsigned int len,
        unsigned offset);

unsigned int queue_size(struct queue *queue);

unsigned int queue_enqueued_size(struct queue *queue);

unsigned int queue_avail_size(struct queue *queue);

int queue_is_empty(struct queue *queue);

int queue_is_full(struct queue *queue);

void queue_reset(struct queue *queue);

#endif
