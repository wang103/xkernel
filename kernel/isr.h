#ifndef ISR_H
#define ISR_H

#include "common.h"

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

#endif
