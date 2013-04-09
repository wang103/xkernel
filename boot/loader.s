; Assembler which "glue" between the bootloader and the kernel.

global loader       ; making entry point visible to the linker
global magic        ; used this in kmain
global mbd          ; used this in kmain

extern kmain        ; defined in xkernel.c

; Setting up the Multiboot header - see GRUB docs for details.
MODULEALIGN equ 1<<0                    ; align modules on page boundaries
MEMINFO     equ 1<<1                    ; provide memory map
FLAGS       equ MODULEALIGN | MEMINFO   ; Multiboot 'flag' field
MAGIC       equ 0x1BADB002              ; 'magic number' for the loader
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum required

section .text

align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; Reserve initial kernel stack space.
STACKSIZE   equ 0x4000              ; 16k

loader:
    mov esp, stack + STACKSIZE      ; set up the stack
    mov [magic], eax                ; Multiboot magic number
    mov [mbd], ebx                  ; Multiboot info structure

    call kmain                      ; call the kernel

    cli                             ; diable the interrupt
.hang:
    hlt                             ; halt machine should kernel return
    jmp .hang

section .bss        ; uninitialized data section

align 4
stack:  resb    STACKSIZE       ; reserve 16kb stack
magic:  resd    1               ; reserve 1 DB
mbd:    resd    1               ; reserve 1 DB
