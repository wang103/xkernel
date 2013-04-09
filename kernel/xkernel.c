#include "common.h"

/**
 * Print the test message on screen to indicate the kernel has successfully
 * loaded and run.
 */
void print_test_msg(void) {
    
    volatile unsigned char *videoram = (unsigned char *)0xB8000;

    videoram[0] = 'W';
    videoram[1] = 0x09;
    videoram[2] = 'e';
    videoram[3] = 0x09;
    videoram[4] = 'l';
    videoram[5] = 0x09;
    videoram[6] = 'c';
    videoram[7] = 0x09;
    videoram[8] = 'o';
    videoram[9] = 0x09;
    videoram[10] = 'm';
    videoram[11] = 0x09;
    videoram[12] = 'e';
    videoram[13] = 0x09;
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
