#include "descriptor_tables.h"
#include "isr.h"

// ASM function that will load the GDT pointer.
extern void gdt_flush(uint32_t);

// ASM function that will load the IDT pointer.
extern void idt_flush(uint32_t);

static void init_gdt();
static void gdt_set_gate(int32_t, uint32_t, uint32_t, uint8_t, uint8_t);

static void init_idt();
static void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);

gdt_entry_struct    gdt_entries[5];
gdt_ptr_struct      gdt_ptr;
idt_entry_struct    idt_entries[256];
idt_ptr_struct      idt_ptr;

/**
 * Initialize the GDT and IDT.
 */
void init_descriptor_tables() {
    init_gdt();
    init_idt();
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

/**
 * Reinitialize/remap the PIC master/slave IRQs.
 *
 * offset1: IRQ offset for the master PIC.
 * offset2: IRQ offset for the slave PIC.
 */
static void PIC_remap(int offset1, int offset2) {
    uint8_t a1, a2;

    // Save masks.
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, PIC_INIT);
    outb(PIC2_COMMAND, PIC_INIT);
    
    // Vector offsets.
    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);

    // How master and slave are wired.
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // Additional info about the environment.
    outb(PIC1_DATA, PIC_8086);
    outb(PIC2_DATA, PIC_8086);

    // Restore saved masks.
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

/**
 * Initialize the IDT.
 */
static void init_idt() {
    idt_ptr.limit = (sizeof(idt_entry_struct) * 256) - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    // CPU-used ISR.
    idt_set_gate( 0, (uint32_t)isr0 , 0x08, 0xEE);
    idt_set_gate( 1, (uint32_t)isr1 , 0x08, 0xEE);
    idt_set_gate( 2, (uint32_t)isr2 , 0x08, 0xEE);
    idt_set_gate( 3, (uint32_t)isr3 , 0x08, 0xEE);
    idt_set_gate( 4, (uint32_t)isr4 , 0x08, 0xEE);
    idt_set_gate( 5, (uint32_t)isr5 , 0x08, 0xEE);
    idt_set_gate( 6, (uint32_t)isr6 , 0x08, 0xEE);
    idt_set_gate( 7, (uint32_t)isr7 , 0x08, 0xEE);
    idt_set_gate( 8, (uint32_t)isr8 , 0x08, 0xEE);
    idt_set_gate( 9, (uint32_t)isr9 , 0x08, 0xEE);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0xEE);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0xEE);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0xEE);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0xEE);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0xEE);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0xEE);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0xEE);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0xEE);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0xEE);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0xEE);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0xEE);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0xEE);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0xEE);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0xEE);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0xEE);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0xEE);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0xEE);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0xEE);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0xEE);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0xEE);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0xEE);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0xEE);

    // Remap the 16 IRQ to isr32 ~ 47.
    PIC_remap(PIC_BASE_IO_MASTER, PIC_BASE_IO_SLAVE);

    idt_set_gate(32, (uint32_t)irq0 , 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1 , 0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2 , 0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3 , 0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4 , 0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5 , 0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6 , 0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7 , 0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8 , 0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9 , 0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}

/**
 * Set the value of an IDT entry.
 */
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel,
        uint8_t flags) {
    idt_entries[num].base_lo    = base & 0xFFFF;
    idt_entries[num].base_hi    = (base >> 16) & 0xFFFF;

    idt_entries[num].sel        = sel;
    idt_entries[num].always0    = 0;

    idt_entries[num].flags      = flags;
}
