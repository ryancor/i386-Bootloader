#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../headers/ports.h"
#include "../headers/mouse.h"
#include "../headers/shell.h"
#include "../headers/screen.h"
#include "../headers/kernel.h"
#include "../headers/keyboard_map.h"
#include "../grub_framework/headers/s_string.h"

// key map from header file
extern unsigned char keyboard_map[128];

// load defined functions from ASM file
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

// IDT_entry defined in header file
struct IDT_entry IDT[IDT_SIZE];

void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	// populate IDT entry of keyboard's interrupt
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;


	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	// ICW1 - begin initialization
	write_port(0x20, 0x11);
	write_port(0xA0, 0x11);

	// ICW2 - remap offset address of IDT
	write_port(0x21, 0x20);
	write_port(0xA1, 0x28);

	// ICW3 - setup cascading
	write_port(0x21, 0x00);
	write_port(0xA1, 0x00);

	// ICW4 - environment info
	write_port(0x21, 0x01);
	write_port(0xA1, 0x01);

	// Initialization finished

	// mask interrupts
	write_port(0x21, 0xff);
	write_port(0xA1, 0xff);

	// fill the IDT descriptor
	idt_address = (unsigned long)IDT;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16;

	load_idt(idt_ptr);
}

void kb_init(void)
{
	// 0xFD is 11111101 - enables only IRQ1 (keyboard)
	write_port(0x21, 0xFD);
}

void keyboard_handler_main(void)
{
	unsigned char status;
	char key[2];
	char keycode;
	char temp_key_string_holder[1024];
	const char *temp_keycode;

	/* write EOI */
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01)
	{
		keycode = read_port(KEYBOARD_DATA_PORT);
		if(keycode < 0)
		{
			return;
		}

		if(keycode == ENTER_KEY_CODE)
		{
			kprint_newline();
			terminal_commands(temp_key_string_holder);
			flush(temp_key_string_holder);

			kprint_newline();
			kprint("> ", 0x09); // terminal cursor
			return;
		}

		if(keycode == ENTER_BACKSPACE_CODE)
		{
			backspace(temp_key_string_holder);
			kprint_backspace();
		}

		// keycode will return a integer that needs to be converted to position
		// for keyboard map
		//vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
		//vidptr[current_loc++] = 0x07;
		char letter = keyboard_map[(int) keycode];
		char str[2] = {letter, '\0'};
		kprint(str, 0x07);

		// copy to buffer
		temp_keycode = str;
		s_strncpy(key, &temp_keycode[0], 1);
		strcat(temp_key_string_holder, key);
	}
}

void cmain(void)
{
	const char *str = "[1.11] My Grubby Kernel";
	const char *str2 = "[2.22]  What to do next? :)";

	clear_screen();

	// write first string
	kprint(str, 0x07);
	kprint_newline();

	// delay and break new line
	delay();

	// write second string
	kprint(str2, 0x03);
	kprint("!~", 0x12);

	delay();

	// test the interrupts
	isr_install();

	__asm__ __volatile__("int $2");
	__asm__ __volatile__("int $3");
	// end of test

	delay();

	return;
}

void kmain(void)
{
	const char *str = "[+] loading kernel drivers";
	const char *str2 = "[3.33] ... Finished Booting ...";

	clear_screen();

	kprint(str, 0x05);
	delay();

	mouse_install();

	__asm__ __volatile__("int $4");
	delay();

	kprint_newline();
	kprint(str2, 0x06);

	kprint_newline();
	kprint_newline();
	kprint_newline();

	// start keyboard
	idt_init();
	kb_init();

	while(1);
}
