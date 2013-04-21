CC=gcc
CFLAGS=-Wall -g

all: test

clean:
	rm test

run:
	./test

test: ../kernel/common.h test.c linked_list.h queue.h queue.c rbtree.h rbtree.c
	$(CC) $(CFLAGS) -o $@ test.c queue.c rbtree.c
