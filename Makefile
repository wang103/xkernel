SOURCES=loader.o xkernel.o

CC=gcc
CFLAGS=-Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs -m32 -c
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

xkernel.o: kernel/xkernel.c
	$(CC) $(CFLAGS) -o $@ kernel/xkernel.c
