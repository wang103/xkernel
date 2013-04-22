#include "../include/multiboot.h"
#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "keyboard.h"
#include "mm.h"

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

    init_mm(phys_mem_size);
    init_descriptor_tables();
    init_timer(TIMER_DEFAULT_FREQ_HZ);
    init_keyboard();
    monitor_clear();

    print_welcome_msg();
    enable_interrupts();

    //test_interrupts();
    //test_paging();

    // Don't ever exit the kernel.
    for (; ;);
}
