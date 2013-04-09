#include "common.h"
#include "monitor.h"

/**
 * Print the test message on screen to indicate the kernel has successfully
 * loaded and run.
 */
void print_test_msg(void) {
    monitor_clear();
    monitor_put("Welcome to xkernel\n");
    monitor_put("This project is currently being developed by Tianyi Wang.\n");
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

    print_test_msg();
}
