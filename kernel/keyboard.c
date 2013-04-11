#include "keyboard.h"
#include "isr.h"
#include "monitor.h"

static void keyboard_callback(registers regs) {
    monitor_put("Key pressed\n");
}

void init_keyboard() {
    // Register the timer callback.
    register_interrupt_handler(IRQ1, &keyboard_callback);
}
