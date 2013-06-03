CC=gcc
CFLAGS=-Wall -g

all: test

clean:
	rm test

run:
	./test

test: ../kernel/common.h test.c linked_list.h rbtree.h rbtree.c map.h map.c
	$(CC) $(CFLAGS) -o $@ test.c rbtree.c map.c
