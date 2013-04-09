#ifndef ISR_H
#define ISR_H

#include "common.h"

typedef struct _registers {
    uint32_t ds;                    // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;    // Pushed by pusha
    uint32_t int_no, err_code;      // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss;  // Push by the processor
} registers;

#endif
