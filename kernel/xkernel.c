#include "../include/multiboot.h"
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "keyboard.h"
#include "mm.h"
#include "kheap.h"
#include "scheduler.h"
#include "../data_struct/queue.h"
#include "../data_struct/map.h"

/**
 * Print the welcome message on screen to indicate the kernel has successfully
 * loaded and run.
 */
static void print_welcome_msg(void) {
    monitor_put("Welcome to xkernel\n");
    monitor_put("This project is currently being developed by Tianyi Wang.\n");
}

/**
 * Trigger some interrupts to make sure ISRs are working correctly.
 */
void test_interrupts() {
    monitor_put("\nTrigger some interrupts for testing purpose:\n");
    asm volatile ("int $0x20");
}

/**
 * Trigger some page fault to make sure it's handled correctly.
 */
void test_paging() {
    uint32_t *ptr = (uint32_t *)0xA0000000;
    *ptr = 0;
}

uint32_t _test_heap_alloc(uint32_t size, int align) {
    uint32_t phys_mem_addr;
    uint32_t vir_mem_addr = kmalloc(size, align, &phys_mem_addr);

    monitor_put("Physical memory at 0x");
    monitor_puthex(phys_mem_addr);
    monitor_put(", virtual memory at 0x");
    monitor_puthex(vir_mem_addr);
    monitor_put(", size=");
    monitor_putdec(size);
    monitor_put(", align=");
    monitor_putdec(align);
    monitor_put("\n");

    return vir_mem_addr;
}

void test_heap() {
    uint32_t overhead = sizeof(mem_node);

    uint32_t v0 = _test_heap_alloc(4096, 0);
    
    uint32_t v1 = _test_heap_alloc(4096, 0);
    if (v0 + 4096 + overhead != v1) {
        PANIC("test_heap: 1");
    }

    uint32_t v2 = _test_heap_alloc(4096, 0);
    if (v1 + 4096 + overhead != v2) {
        PANIC("test_heap: 2");
    }

    uint32_t v3 = _test_heap_alloc(4, 0);
    if (v2 + 4096 + overhead != v3) {
        PANIC("test_heap: 3");
    }

    uint32_t v4 = _test_heap_alloc(1024, 1);
    if (v4 < v3 || v4 & (~MM_ALIGN_4K)) {
        PANIC("test_heap: 4");
    }

    uint32_t v5 = _test_heap_alloc(2048, 1);
    if (v5 < v4 || v5 & (~MM_ALIGN_4K)) {
        PANIC("test_heap: 5");
    }

    uint32_t v6 = _test_heap_alloc(3072, 0);
    if (v6 > v4 || !(v6 & (~MM_ALIGN_4K))) {
        PANIC("test_heap: 6");
    }

    // Start freeing.
    kfree((void *)v3);
    kfree((void *)v1);
    kfree((void *)v0);
    kfree((void *)v6);
    kfree((void *)v4);
    kfree((void *)v5);
    kfree((void *)v2);

    monitor_put("Heap tests passed!\n");
}

void test_queue() {
    unsigned int i;

    struct queue fifo;
    queue_alloc(&fifo, 32 * sizeof(unsigned int), 1);

    if (!queue_is_empty(&fifo)) {
        PANIC("test queue: 0");
    }

    for (i = 0; i < 32; i++) {
        queue_in(&fifo, &i, sizeof(i));
    }

    if (queue_enqueued_size(&fifo) != 32 * sizeof(unsigned int)) {
        PANIC("test queue: 1");
    }

    if (!queue_is_full(&fifo)) {
        PANIC("test queue: 5");
    }

    i = 0;
    while (queue_enqueued_size(&fifo)) {
        unsigned int val;
        
        queue_out_peek(&fifo, &val, sizeof(unsigned int), 0);
        if (val != i) {
            monitor_putdec(val);
            monitor_put("\n");
            monitor_putdec(i);
            monitor_put("\n");
            PANIC("test queue: 2");
        }

        queue_out(&fifo, &val, sizeof(unsigned int));
        if (val != i) {
            monitor_putdec(val);
            monitor_put("\n");
            monitor_putdec(i);
            monitor_put("\n");
            PANIC("test queue: 3");
        }

        i++;
    }

    if (queue_is_full(&fifo)) {
        PANIC("test queue: 4");
    }
}

void test_map() {
    struct map map_test;
    map_init(&map_test);

    add(&map_test, (void *)0, 0);
    add(&map_test, (void *)5, 5);
    add(&map_test, (void *)6, 6);
    
    int actual0 = (int)lookup(&map_test, 0);
    int actual1 = (int)lookup(&map_test, 5);
    int actual2 = (int)lookup(&map_test, 6);

    if (actual0 != 0) {
        PANIC("test map: 0");
    }
    if (actual1 != 5) {
        PANIC("test map: 1");
    }
    if (actual2 != 6) {
        PANIC("test map: 2");
    }

    int new_id;
    allocate(&map_test, (void *)1, &new_id);

    if (new_id != 1) {
        PANIC("test map: 3");
    }
    
    int actual3 = (int)lookup(&map_test, 1);
    if (actual3 != 1) {
        PANIC("test map: 4");
    }
    
    map_destroy(&map_test);
}

typedef struct multiboot_memory_map {
    unsigned int size;
    unsigned int base_addr_low, base_addr_high;
    unsigned int length_low, length_high;
    unsigned int type;
} multiboot_memory_map_t;

static unsigned int get_phys_mem_size(multiboot_info_t *mbt) {
    return mbt->mem_upper * MM_1K + 1048576 /* 1MB */;
}

/**
 * The starting function of the kernel.
 */
void kmain(void) {

    extern uint32_t magic;
    // Uncomment the following if access to multiboot header is needed.
    extern void *mbd;

    if (magic != 0x2BADB002) {
        // Something went wrong according to specs.
        // Return to halt.
        return;
    }

    unsigned int phys_mem_size = get_phys_mem_size(mbd);
    if (phys_mem_size <= 0) {
        // RAM fail. Return to halt.
        return;
    }

    init_descriptor_tables();
    init_mm(phys_mem_size);
    init_kheap();
    init_timer(TIMER_DEFAULT_FREQ_HZ);
    init_keyboard();
    monitor_clear();
    init_scheduler();

    print_welcome_msg();
    enable_interrupts();

    //test_interrupts();
    //test_paging();
    //test_heap();
    //test_queue();
    //test_map();

    // Don't ever exit the kernel.
    for (; ;);
}
