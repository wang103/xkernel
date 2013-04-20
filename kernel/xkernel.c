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

/**
 * The starting function of the kernel.
 */
void kmain(void) {

    extern uint32_t magic;
    // Uncomment the following if access to multiboot header is needed.
    //extern void *mbd;

    if (magic != 0x2BADB002) {
        // Something went wrong according to specs.
        // Return to halt.
        return;
    }

    init_descriptor_tables();
    init_mm();
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
