#include "isr.h"
#include "monitor.h"

void isr_handler(registers regs) {
    monitor_put("Received interrupt: ");
    monitor_putdec(regs.int_no);
    monitor_putchar('\n');
    monitor_put("The error code is: ");
    monitor_putdec(regs.err_code);
    monitor_putchar('\n');

    // Interrupts generated externally by INT instruction will not have the
    // error code pushed onto the stack, this will mess up the stack, so for
    // the interrupts that have error codes, this ISR handler will just halt
    // instead of return.
    if (regs.int_no == 8 || (regs.int_no >= 10 && regs.int_no <=14)) {
        for (;;);
    }
}
