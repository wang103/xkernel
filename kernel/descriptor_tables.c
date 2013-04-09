#include "descriptor_tables.h"

// ASM function that will load the GDT pointer.
extern void gdt_flush(uint32_t);

static void init_gdt();
static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_struct    gdt_entries[5];
gdt_ptr_struct      gdt_ptr;
idt_entry_struct    idt_entries[256];
idt_ptr_struct      idt_ptr;

/**
 * Initialize the GDT and IDT.
 */
void init_descriptor_tables() {
    // Initialize the GDT.
    init_gdt();
}

/**
 * Initialize the GDT.
 */
static void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_struct) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // 4KB page granularity and 32 bit protected mode for all segments.

    gdt_set_gate(0, 0, 0, 0, 0);                    // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);     // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);     // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);     // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);     // User mode data segment

    gdt_flush((uint32_t)&gdt_ptr);
}

/**
 * Set the value of a GDT entry.
 */
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit,
        uint8_t access, uint8_t granularity) {
    gdt_entries[num].base_low       = (base & 0xFFFF);
    gdt_entries[num].base_middle    = (base >> 16) & 0xFF;
    gdt_entries[num].base_high      = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low      = (limit & 0xFFFF);
    gdt_entries[num].granularity    = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity   |= granularity & 0xF0;
    gdt_entries[num].access         = access;
}
