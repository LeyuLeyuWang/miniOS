#ifndef MINI_ISR_H
#define MINI_ISR_H

#include <stdint.h>

// Interrupt Service Routine installer
void isr_install(void);

// Interrupt handler type
struct regs {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

typedef void (*isr_t)(struct regs *r);

#endif // MINI_ISR_H
