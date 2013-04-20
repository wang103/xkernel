CC=gcc
CFLAGS=-Wall -g

all: test

clean:
	rm test

run:
	./test

test: test.c linked_list.h ../kernel/common.h
	$(CC) $(CFLAGS) -o test test.c
