SOURCES=loader.o xkernel.o common.o monitor.o

CC=gcc
CFLAGS=-Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs \
	   -nostdinc -fno-builtin -fno-stack-protector -m32 -c
LD=ld
LDFLAGS=-T boot/linker.ld -melf_i386
AS=nasm
ASFLAGS=-felf

all: xkernel

clean:
	rm *.o xkernel

run:
	qemu-system-i386 -kernel xkernel

xkernel: $(SOURCES) boot/linker.ld
	$(LD) $(LDFLAGS) -o $@ $(SOURCES)

loader.o: boot/loader.s
	$(AS) $(ASFLAGS) -o $@ boot/loader.s

xkernel.o: kernel/xkernel.c kernel/common.h kernel/monitor.h
	$(CC) $(CFLAGS) -o $@ kernel/xkernel.c

common.o: kernel/common.h kernel/common.c
	$(CC) $(CFLAGS) -o $@ kernel/common.c

monitor.o: kernel/monitor.h kernel/monitor.c kernel/common.h
	$(CC) $(CFLAGS) -o $@ kernel/monitor.c
