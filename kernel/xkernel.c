/**
 * Print the test message on screen to indicate the kernel has successfully
 * loaded and run.
 */
void print_test_msg(void) {
    
    volatile unsigned char *videoram = (unsigned char *)0xB8000;

    videoram[0] = 'W';
    videoram[1] = 0x07;
    videoram[2] = 'e';
    videoram[3] = 0x07;
    videoram[4] = 'l';
    videoram[5] = 0x07;
    videoram[6] = 'c';
    videoram[7] = 0x07;
    videoram[8] = 'o';
    videoram[9] = 0x07;
    videoram[10] = 'm';
    videoram[11] = 0x07;
    videoram[12] = 'e';
}

void kmain(void) {

    extern uint32_t magic;
    extern void *mbd;

    if (magic != 0x2BADB002) {
        // Something went wrong according to specs.
        // Return to halt.
        return;
    }

    print_test_msg();
}
