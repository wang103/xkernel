#include "keyboard.h"
#include "isr.h"
#include "monitor.h"

static void keyboard_callback(registers regs) {
    uint8_t scan_code;

    monitor_put("Key pressed\n");
    
    // Read the keyboard's data buffer.
    scan_code = inb(KEYBOARD_BUFFER_PORT);
}

void init_keyboard() {
    // Register the timer callback.
    register_interrupt_handler(IRQ1, &keyboard_callback);
}
