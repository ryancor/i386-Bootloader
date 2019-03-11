#include "timer.h"
#include "isr.h"
#include "../headers/ports.h"
#include "../headers/screen.h"
#include "../grub_framework/headers/s_string.h"

u32 tick = 0;

static void timer_callback(registers_t regs)
{
	tick++;
	kprint("Tick: ", 0x07);

	char tick_ascii[256];
	int_to_ascii(tick, tick_ascii);
	kprint(tick_ascii, 0x08);
	kprint("\n", 0x07);
}

void init_timer(u32 freq)
{
	register_interrupt_handler(IRQ0, timer_callback);

	// Get the PIT value: hardware clock at 1193180Hz
	u32 divisor = 1193180 / freq;
	u8 low = (u8)(divisor & 0xFF);
	u8 high = (u8)((divisor >> 8) & 0xFF);
	// Send the command
	port_byte_out(0x43, 0x36); // Command port
	port_byte_out(0x40, low);
	port_byte_out(0x40, high);
}
