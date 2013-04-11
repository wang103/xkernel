#include "isr.h"
#include "monitor.h"

/**
 * Send the EOI command to the PIC chip.
 */
static void PIC_send_EOI(uint8_t irq) {
    if (irq >= PIC_BASE_IO_SLAVE) {
        // Send EOI to the slave PIC.
        outb(PIC2_COMMAND, PIC_EOI);
    }

    // Send EOI to the master PIC.
    outb(PIC1_COMMAND, PIC_EOI);
}

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

void irq_handler(registers regs) {
    PIC_send_EOI(regs.int_no);
    
    monitor_put("Received interrupt request from PIC: ");
    monitor_putdec(regs.int_no);
    monitor_putchar('\n');

    // Now handle the IRQ.

}
