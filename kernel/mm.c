#include "mm.h"

extern uint32_t end;        // Defined in the linker script, start of heap
uint32_t placement_address = (uint32_t)&end;    // For heap allocation

// Bitmap for frames - used or free.
uint32_t *frames_bitmap;
uint32_t frames_num;        // Total number of frames

/**
 * Set a bit in the frame bitmap.
 */
static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / MM_4K;
    uint32_t index = BYTE_INDEX_FROM_BIT(frame);
    uint32_t offset = BIT_OFFSET_FROM_BIT(frame);

    frames_bitmap[index] |= (0x1 << offset);
}

/**
 * Clear a bit in the frame bitmap.
 */
static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / MM_4K;
    uint32_t index = BYTE_INDEX_FROM_BIT(frame);
    uint32_t offset = BIT_OFFSET_FROM_BIT(frame);

    frames_bitmap[index] &= ~(0x1 << offset);
}

/**
 * Test if a bit is set in the frame bitmap.
 */
static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / MM_4K;
    uint32_t index = BYTE_INDEX_FROM_BIT(frame);
    uint32_t offset = BIT_OFFSET_FROM_BIT(frame);

    return frames_bitmap[index] & (0x1 << offset);
}

/**
 * Find the first available frame.
 * Return the index of the frame. Return -1 if no frames are available.
 */
static uint32_t get_first_free_frame() {
    uint32_t i, j;
    for (i = 0; i < BYTE_INDEX_FROM_BIT(frames_num); i++) {
        if (frames_bitmap[i] != 0xFFFFFFFF) {
            // At least one bit is free here.
            for (j = 0; j < 32; j++) {
                if (!(frames_bitmap[i] & (0x1 << j))) {
                    return i * 32 + j;
                }
            }
        }
    }
    return (uint32_t)-1;
}

/**
 * Allocate a frame for a page.
 */
void alloc_frame(page *page, int is_kernel, int is_writable) {
    if (page->frame != 0) {
        // Frame was already allocated, just return.
        return;
    }

    uint32_t index = get_first_free_frame();
    if (index == (uint32_t)-1) {
        // No more available frame, just return.
        return;
    }

    set_frame(index * MM_4K);       // Mark the frame as allocated
    page->present = 1;
    page->rw = (is_writable ? 1 : 0);
    page->user = (is_kernel ? 0 : 1);
    page->accessed = 0;
    page->dirty = 0;
    page->frame = index;
}

/**
 * Free a frame corresponding to the page.
 */
void free_frame(page *page) {
    uint32_t frame = page->frame;
    if (!frame) {
        // The given page doesn't have a frame allocated.
        return;
    }

    clear_frame(frame * MM_4K);
    page->present = 0;
    page->frame = 0x0;
}

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