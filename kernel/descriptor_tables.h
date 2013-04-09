#ifndef DESCRIPTOR_TABLES_H
#define DESCRIPTOR_TABLES_H

#include "common.h"

struct _gdt_entry_struct {
    uint16_t limit_low;     // the lower 16 bits of the limit.
    uint16_t base_low;      // the lower 16 bits of the base.
    uint8_t base_middle;    // the next 8 bits of the base.
    uint8_t access;         // access flags.
    uint8_t granularity;
    uint8_t base_high;      // the last 8 bits of the base.
} __attribute__((packed));

typedef struct _gdt_entry_struct gdt_entry_struct;

struct _gdt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

typedef struct _gdt_ptr_struct gdt_ptr_struct;

struct _idt_entry_struct {
    uint16_t base_lo;       // The lower 16 bits of the address to jump to.
    uint16_t sel;           // Kernel segment selector.
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;       // The upper 16 bits of the address to jump to.
} __attribute__((packed));

typedef struct _idt_entry_struct idt_entry_struct;

struct _idt_ptr_struct {
    uint16_t limit;
    uint32_t base;          // The address of the first element.
} __attribute__((packed));

typedef struct _idt_ptr_struct idt_ptr_struct;

void init_descriptor_tables();

// The extern directives for accessing ASM ISR handlers.
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

#endif
