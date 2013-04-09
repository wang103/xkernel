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

void init_descriptor_tables();

#endif
