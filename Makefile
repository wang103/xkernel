SOURCES=loader.o desc_tables.o interrupt.o pic.o xkernel.o common.o monitor.o \
		descriptor_tables.o isr.o timer.o

CC=gcc
CFLAGS=-Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -nostdinc \
	   -fno-builtin -fno-stack-protector -m32 -c
LD=ld
LDFLAGS=-T scripts/linker.ld -melf_i386
AS=nasm
ASFLAGS=-felf

all: xkernel

clean:
	rm *.o xkernel

run:
	qemu-system-i386 -kernel xkernel

xkernel: $(SOURCES) scripts/linker.ld
	$(LD) $(LDFLAGS) -o $@ $(SOURCES)

loader.o: arch/x86/loader.s
	$(AS) $(ASFLAGS) -o $@ arch/x86/loader.s

desc_tables.o: arch/x86/desc_tables.s
	$(AS) $(ASFLAGS) -o $@ arch/x86/desc_tables.s

interrupt.o: arch/x86/interrupt.s
	$(AS) $(ASFLAGS) -o $@ arch/x86/interrupt.s

pic.o: arch/x86/pic.s
	$(AS) $(ASFLAGS) -o $@ arch/x86/pic.s

xkernel.o: kernel/xkernel.c kernel/common.h kernel/monitor.h
	$(CC) $(CFLAGS) -o $@ kernel/xkernel.c

common.o: kernel/common.h kernel/common.c
	$(CC) $(CFLAGS) -o $@ kernel/common.c

monitor.o: kernel/monitor.h kernel/monitor.c kernel/common.h
	$(CC) $(CFLAGS) -o $@ kernel/monitor.c

descriptor_tables.o: kernel/descriptor_tables.h kernel/descriptor_tables.c \
	kernel/common.h kernel/isr.h
	$(CC) $(CFLAGS) -o $@ kernel/descriptor_tables.c

isr.o: kernel/isr.h kernel/isr.c kernel/common.h kernel/monitor.h
	$(CC) $(CFLAGS) -o $@ kernel/isr.c

timer.o: kernel/timer.h kernel/timer.c kernel/isr.h kernel/monitor.h
	$(CC) $(CFLAGS) -o $@ kernel/timer.c
