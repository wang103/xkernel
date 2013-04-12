#include "mm.h"

uint32_t placement_address = 0;

/**
 * Allocate memory in the early stage of the kernel -- before the heap is
 * active. Memory allocated here won't be freed.
 *
 * Upon return, the memory address will be page-align if 'align' is 1. And if
 * 'phys' is not NULL, the physical address will be stored in 'phys'.
 */
uint32_t kmalloc_early(uint32_t size, int align, uint32_t *phys) {
    if (align && (placement_address & MM_ALIGN_4K)) {
        placement_address &= MM_ALIGN_4K;
        placement_address += MM_4K;
    }

    if (phys) {
        *phys = placement_address;
    }

    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}
