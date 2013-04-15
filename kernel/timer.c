#include "timer.h"
#include "isr.h"
#include "monitor.h"

uint32_t tick = 0;

static void timer_callback(registers regs) {
    if (regs.err_code) {
        PANIC("TIMER");
    }

    tick++;
}

/**
 * Initialize the timer with the provided frequency in Hz.
 */
void init_timer(uint32_t freq) {
    // Register the timer callback.
    register_interrupt_handler(IRQ0, &timer_callback);

    uint32_t divisor = CLOCK_MAX_FREQ_HZ / freq;
    
    outb(PIT_COMMAND_PORT, PIT_MODE_REPEATING);

    // Send the divisor byte-wise.
    uint8_t lo = (uint8_t)(divisor & 0xFF);
    uint8_t hi = (uint8_t)((divisor >> 8) & 0xFF);

    outb(PIT_DATA_0_PORT, lo);
    outb(PIT_DATA_0_PORT, hi);
}
