#include "mm.h"
#include "kheap.h"

extern uint32_t end;        // Defined in the linker script, start of heap
uint32_t placement_address = (uint32_t)&end;    // For heap allocation
extern uint32_t kheap_cur_end;
extern uint32_t kheap_end;
extern page_directory *kernel_directory;

// Bitmap for frames - used or free.
uint32_t *frames_bitmap;
uint32_t frames_num;        // Total number of frames

/**
 * init_mem - initialize memory related things.
 * @phys_mem_size: physical memory size in byte.
 */
void init_mm(uint32_t phys_mem_size) {
    kheap_end = KHEAP_START + phys_mem_size / 8;

    frames_num = phys_mem_size / MM_4K;
    frames_bitmap = (uint32_t *)kmalloc(frames_num / 8, 0, NULL);

    // Clear the bitmap.
    memset((uint8_t *)frames_bitmap, 0, frames_num / 8);

    initialize_paging();
}

/**
 * Set a bit in the frame bitmap.
 */
static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / MM_4K;
    uint32_t index = INT_INDEX_FROM_BIT(frame);
    uint32_t offset = BIT_OFFSET_FROM_BIT(frame);

    frames_bitmap[index] |= (0x1 << offset);
}

/**
 * Clear a bit in the frame bitmap.
 */
static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / MM_4K;
    uint32_t index = INT_INDEX_FROM_BIT(frame);
    uint32_t offset = BIT_OFFSET_FROM_BIT(frame);

    frames_bitmap[index] &= ~(0x1 << offset);
}

/**
 * Test if a bit is set in the frame bitmap.
 */
/*Not need
static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / MM_4K;
    uint32_t index = INT_INDEX_FROM_BIT(frame);
    uint32_t offset = BIT_OFFSET_FROM_BIT(frame);

    return frames_bitmap[index] & (0x1 << offset);
}
*/

/**
 * Find the first available frame.
 * Return the index of the frame. Return -1 if no frames are available.
 */
static uint32_t get_first_free_frame() {
    uint32_t i, j;
    for (i = 0; i < INT_INDEX_FROM_BIT(frames_num); i++) {
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
        // Frame was already allocated for this page, just return.
        return;
    }

    uint32_t index = get_first_free_frame();
    if (index == (uint32_t)-1) {
        // No more available frame, kernel panic.
        PANIC("No more frame");
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
        // The given page doesn't have a frame allocated for it.
        return;
    }

    clear_frame(frame * MM_4K);
    page->present = 0;
    page->frame = 0x0;
}

/**
 * During early stage of the kernel, when heap is not active yet, this function
 * allocates memory by increasing a counter. Memory allocated this way won't be
 * freed.
 *
 * Upon return, the memory address will be page-align if 'align' is 1. And if
 * 'phys' is not NULL, the physical address will be stored in 'phys'.
 */
uint32_t kmalloc(uint32_t size, int align, uint32_t *phys) {
    uint32_t addr;

    if (kheap_cur_end != 0) {
        // Heap has been established, and placement_address has been frozen.
        addr = (uint32_t)alloc(size, align);

        if (phys) {
            page *pg = get_page(addr, 1, kernel_directory);
            *phys = (pg->frame << 12) + (addr & 0xFFF);
        }
    }
    else {
        if (align && (placement_address & (~MM_ALIGN_4K))) {
            // Align it if it's not already.
            placement_address &= MM_ALIGN_4K;
            placement_address += MM_4K;
        }

        if (phys) {
            *phys = placement_address;
        }

        addr = placement_address;
        placement_address += size;
    }

    return addr;
}

void kfree(void *addr) {
    free(addr);
}
