#include "isr.h"
#include "monitor.h"

isr_h interrupt_handlers[256];

static void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if (IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }

    value = inb(port) | (1 << IRQline);
    outb(port, value);
}
 
static void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if (IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);
}

void register_interrupt_handler(uint8_t isr_num, isr_h handler) {
    interrupt_handlers[isr_num] = handler;
    if (isr_num >= IRQ0) {
        IRQ_clear_mask(isr_num - IRQ0);
    }
}

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
    monitor_put("IRQ triggered\n");

    // Now handle the IRQ.
    isr_h handler = interrupt_handlers[regs.int_no];
    if (handler != NULL) {
        handler(regs);
    } else {
        monitor_put("No supported handler for this IRQ");
    }

    PIC_send_EOI(regs.int_no);
}
