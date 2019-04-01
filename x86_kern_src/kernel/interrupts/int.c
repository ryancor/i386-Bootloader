#include "headers/int.h"

// Returns true boolean if irq are enabled for the CPU
bool are_interrupts_enabled()
{
	unsigned long flags;
	asm volatile("pushf\n\t"
		     "pop %0"
		     : "=g"(flags));
	return flags & (1 << 9);
}

// push/pop interrupt flags
static inline unsigned long save_irqdisable(void)
{
	unsigned long flags;
	asm volatile("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
}

void irqrestore(unsigned long flags)
{
	asm("push %0\n\tpopf" : : "rm"(flags) : "memory","cc");
}

void intended_usage(void)
{
	unsigned long f = save_irqdisable();
	// do something with irq's
	irqrestore(f);
}

// Request for CPU Identification
void cpuid(int code, uint32_t *a, uint32_t *d)
{
	asm volatile("cpuid" : "=a"(*a), "=d"(*d) : "0"(code) : "ebx", "ecx");
}

// Read the value in a control register
static inline unsigned long read_cr0(void)
{
	unsigned long val;
	asm volatile("mov %%cr0, %0" : "=r"(val));
	return val;
}
