#include "fpu.h"
#include "../../cpu/isr.h"
#include "../../headers/system.h"

#define NO_LAZY_FPU

process_t *fpu_thread = NULL;
process_t *current_process = NULL;

void set_fpu_cw(const uint16_t cw)
{
	asm volatile("fldcw %0" :: "m"(cw));
}

void enable_fpu(void)
{
	asm volatile("clts");
	size_t t;
	asm volatile("mov %%cr0, %0" : "=r"(t));
	t &= ~(1 << 2);
	t |= (1 << 1);
	asm volatile("mov %0, %%cr0" :: "r"(t));

	asm volatile("mov %%cr4, %0" : "=r"(t));
	t |= 3 << 9;
	asm volatile("mov %0, %%cr4" :: "r"(t));
}

void disable_fpu(void)
{
	size_t t;
	asm volatile("mov %%cr0, %0" : "=r"(t));
	t |= 1 << 3;
	asm volatile("mov %0, %%cr0" :: "r"(t));
}

uint8_t saves[512] __attribute__((aligned(16)));

void restore_fpu(process_t *proc)
{
	memcpy(&saves, (uint8_t *)&proc->thread.fp_regs, 512);
	asm volatile("fxrstor (%0)" :: "r"(saves));
}

void save_fpu(process_t *proc)
{
	asm volatile("fxsave (%0)" :: "r"(saves));
	memcpy((uint8_t*)&proc->thread.fp_regs, &saves, 512);
}

void init_fpu(void)
{
	asm volatile("fninit");
}

void invalid_op(void)
{
	enable_fpu();
	if(fpu_thread == current_process)
	{
		return;
	}

	if(fpu_thread)
	{
		save_fpu(fpu_thread);
	}

	fpu_thread = (process_t *)current_process;
	if(!fpu_thread->thread.fpu_enabled)
	{
		init_fpu();
		fpu_thread->thread.fpu_enabled = 1;
		return;
	}
	restore_fpu(fpu_thread);
}

void switch_fpu(void)
{
#ifdef NO_LAZY_FPU
	save_fpu((process_t *)current_process);
#else
	disable_fpu();
#endif
}

void unswitch_fpu(void)
{
#ifdef NO_LAZY_FPU
        restore_fpu((process_t *)current_process);
#endif
}

void fpu_install(void)
{
#ifdef NO_LAZY_FPU
	enable_fpu();
	init_fpu();
	save_fpu((void*)current_process);
#else
	enable_fpu();
	disable_fpu();
	register_interrupt_handler(7, &invalid_op);
#endif
}
