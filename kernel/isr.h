#ifndef ISR_H
#define ISR_H

#include "common.h"

#define IRQ0        32      // Timer
#define IRQ1        33
#define IRQ2        34
#define IRQ3        35
#define IRQ4        36
#define IRQ5        37
#define IRQ6        38
#define IRQ7        39
#define IRQ8        40
#define IRQ9        41
#define IRQ10       42
#define IRQ11       43
#define IRQ12       44
#define IRQ13       45
#define IRQ14       46
#define IRQ15       47

#define PIC1            0x20    // IO base addr for master PIC
#define PIC2            0xA0    // IO base addr for slave PIC
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1 + 1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2 + 1)

#define PIC_EOI         0x20    // End of interrupt command code
#define PIC_INIT        0x11    // Initialization code
#define PIC_8086        0x01    // 8086/88 (MCS-80/85) mode

#define PIC_BASE_IO_MASTER  0x20
#define PIC_BASE_IO_SLAVE   0x28

typedef struct _registers {
    uint32_t ds;                    // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;    // Pushed by pusha
    uint32_t int_no, err_code;      // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss;  // Push by the processor
} registers;

typedef void (*isr_h)(registers);

void register_interrupt_handler(uint8_t isr_num, isr_h handler);

#endif
