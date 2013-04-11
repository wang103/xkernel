#include "common.h"
#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "keyboard.h"

/**
 * Print the test message on screen to indicate the kernel has successfully
 * loaded and run.
 */
void print_test_msg(void) {
    monitor_put("Welcome to xkernel\n");
    monitor_put("This project is currently being developed by Tianyi Wang.\n");
}

/**
 * Trigger some interrupts to make sure ISRs are working correctly.
 */
void trigger_test_interrupts() {
    monitor_put("\nTrigger some interrupts for testing purpose:\n");
    asm volatile ("int $0x20");
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

    init_timer(TIMER_DEFAULT_FREQ_HZ);
    init_keyboard();
    monitor_clear();
    init_descriptor_tables();

    print_test_msg();
    //trigger_test_interrupts();
}
