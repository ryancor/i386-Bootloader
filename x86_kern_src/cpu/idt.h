#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "../grub_framework/headers/s_stdint.h"

// Segment selectors
#define KERNEL_CS	0x08

typedef struct 
{
	uint16_t low_offset;
	uint16_t sel;
	uint8_t always0;
	uint8_t flags;
	uint16_t high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct 
{
        uint16_t limit;
        uint32_t base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES	256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif
