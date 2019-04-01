#include "../../../grub_framework/headers/s_stdint.h"
#include "../../../grub_framework/headers/s_stdbool.h"

bool are_interrupts_enabled();

void intended_usage(void);

void irqrestore(unsigned long flags);

void cpuid(int code, uint32_t *a, uint32_t *d);

