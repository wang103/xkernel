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
    
    if (isr_num >= IRQ0 && isr_num <= IRQ15) {
        IRQ_clear_mask(isr_num - IRQ0);
    }
}

void unregister_interrupt_handler(uint8_t isr_num) {
    interrupt_handlers[isr_num] = NULL;

    if (isr_num >= IRQ0 && isr_num <= IRQ15) {
        IRQ_set_mask(isr_num - IRQ0);
    }
}

/**
 * Send the EOI command to the PIC chip.
 */
static void PIC_send_EOI(uint8_t irq) {
    if (irq >= PIC_BASE_IO_SLAVE && irq <= IRQ15) {
        // Send EOI to the slave PIC.
        outb(PIC2_COMMAND, PIC_EOI);
    }

    // Send EOI to the master PIC.
    outb(PIC1_COMMAND, PIC_EOI);
}

/**
 * Return 0 if the interrupt is correctly handled.
 */
static int handle_interrupt(registers regs) {
    isr_h handler = interrupt_handlers[regs.int_no];
    if (handler != NULL) {
        handler(regs);
        return 0;
    } else {
        monitor_put("No supported handler for this interrupt");
        return 1;
    }
}

void isr_handler(registers regs) {
    monitor_put("Received interrupt: ");
    monitor_putdec(regs.int_no);
    monitor_putchar('\n');
    monitor_put("The error code is: ");
    monitor_putdec(regs.err_code);
    monitor_putchar('\n');

    // Now handle the interrupt.
    int result = handle_interrupt(regs);

    if (result) {
        monitor_put("Interrupt unhandled, going into infinite loop.\n");
        for (; ;);
    }
}

void irq_handler(registers regs) {
    // Now handle the IRQ.
    int result = handle_interrupt(regs);

    if (result) {
        monitor_put("Interrupt unhandled, going into infinite loop.\n");
        for (; ;);
    }

    PIC_send_EOI(regs.int_no);
}
