[GLOBAL gdt_flush]          ; Used in descriptor_tables.c
[GLOBAL idt_flush]          ; Used in descriptor_tables.c

gdt_flush:
    mov eax, [esp + 4]      ; Get the pointer to the GDT as first parameter
    lgdt [eax]              ; Load the new GDT pointer

    mov ax, 0x10            ; 0x10 is the offset in the GDT to the data segment
    mov ds, ax              ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush         ; 0x08 is the offset in the GDT to the code segment,
                            ; do a far jump to set the cs implicitly.
.flush:
    ret

idt_flush:
    mov eax, [esp + 4]      ; Get the pointer to the IDT as first parameter
    lidt [eax]              ; Load the new IDT pointer
    ret
