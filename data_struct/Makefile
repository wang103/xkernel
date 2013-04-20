CC=gcc
CFLAGS=-Wall -g

all: test

clean:
	rm test

run:
	./test

test: test.c ../kernel/common.h linked_list.h queue.h
	$(CC) $(CFLAGS) -o test test.c
