# The C and C++ rules are setup by default.
# The only one that needs changing is the assembler rule,
# as we use nasm instead of GNU as.

SOURCES=boot/loader.o

CFLAGS=
LD=ld
LDFLAGS=-T link.ld
AS=nasm
ASFLAGS=-f elf

all: $(SOURCES) link

clean:
	rm *.o xkernel

link:
	$(LD) $(LDFLAGS) -o xkernel $(SOURCES)

*.o:
	$(AS) $(ASFLAGS) $<
