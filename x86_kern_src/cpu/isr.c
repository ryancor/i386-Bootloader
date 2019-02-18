#include "isr.h"
#include "idt.h"
#include "../headers/ports.h"
#include "../headers/screen.h"
#include "../grub_framework/headers/s_string.h"

isr_t interrupt_handlers[256];

void isr_install()
{
	set_idt_gate(0, (u32)isr0);
	set_idt_gate(1, (u32)isr1);
	set_idt_gate(2, (u32)isr2);
	set_idt_gate(3, (u32)isr3);
	set_idt_gate(4, (u32)isr4);
	set_idt_gate(5, (u32)isr5);
	set_idt_gate(6, (u32)isr6);
	set_idt_gate(7, (u32)isr7);
	set_idt_gate(8, (u32)isr8);
	set_idt_gate(9, (u32)isr9);
	set_idt_gate(10, (u32)isr10);
	set_idt_gate(11, (u32)isr11);
	set_idt_gate(12, (u32)isr12);
	set_idt_gate(13, (u32)isr13);
	set_idt_gate(14, (u32)isr14);
	set_idt_gate(15, (u32)isr15);
	set_idt_gate(16, (u32)isr16);
	set_idt_gate(17, (u32)isr17);
	set_idt_gate(18, (u32)isr18);
	set_idt_gate(19, (u32)isr19);
	set_idt_gate(20, (u32)isr20);
	set_idt_gate(21, (u32)isr21);
	set_idt_gate(22, (u32)isr22);
	set_idt_gate(23, (u32)isr23);
	set_idt_gate(24, (u32)isr24);
	set_idt_gate(25, (u32)isr25);
	set_idt_gate(26, (u32)isr26);
	set_idt_gate(27, (u32)isr27);
	set_idt_gate(28, (u32)isr28);
	set_idt_gate(29, (u32)isr29);
	set_idt_gate(30, (u32)isr30);
	set_idt_gate(31, (u32)isr31);

	set_idt();
}

char *exception_messages[] =
{
	"Division By Zero",
    	"Debug",
    	"Non Maskable Interrupt",
    	"Breakpoint",
    	"Into Detected Overflow",
    	"Out of Bounds",
    	"Invalid Opcode",
    	"No Coprocessor",

    	"Double Fault",
    	"Coprocessor Segment Overrun",
    	"Bad TSS",
    	"Segment Not Present",
    	"Stack Fault",
    	"General Protection Fault",
    	"Page Fault",
    	"Unknown Interrupt",

    	"Coprocessor Fault",
    	"Alignment Check",
    	"Machine Check",
    	"Reserved",
    	"Reserved",
    	"Reserved",
    	"Reserved",
    	"Reserved",

    	"Reserved",
    	"Reserved",
    	"Reserved",
    	"Reserved",
    	"Reserved",
    	"Reserved",
    	"Reserved",
    	"Reserved"
};

void isr_handler(registers_t r)
{
	kprint("\n", 0x07);
	kprint("received interrupt: ", 0x07);
	char s[3];
	int_to_ascii(r.int_no, s);
	kprint(s, 0x08);
	kprint("\n", 0x07);
	kprint(exception_messages[r.int_no], 0x08);
	kprint("\n", 0x07);
}

void register_interrupt_handler(u8 n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r)
{
	// After every interrupt we need to send an EOI to the PICs
	// or they will not send another interrupt again
	if(r.int_no >= 40)
	{
		port_byte_out(0xA0, 0x20); // slave
	}

	port_byte_out(0x20, 0x20); // master

	// Handle the interrupt in a modular way
	if(interrupt_handlers[r.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[r.int_no];
		handler(r);
	}
}
