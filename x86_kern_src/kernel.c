#include "headers/kernel.h"
#include "headers/keyboard_map.h"

// key map from header file
extern unsigned char keyboard_map[128];

// load defined functions from ASM file
extern void key_handlr(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

// IDT_entry defined in header file
typedef struct IDT_entry IDT[IDT_SIZE];

void idt_init(void) {
	return;
}

void kb_init(void) {
	return;
}

void delay() {
	for(unsigned int n = 0; n < 500000000; n++);
}

void clear_screen() {
	unsigned int j = 0;

	// this loop clears the screen
	// each line takes 2 bytes
	while(j < 80 * 25 * 2) {
		// blank character
		vidptr[j] = ' ';
		// attribute byte
		vidptr[j+1] = 0x07;
		j = j + 2;
	}
}

void write_string(const char *string, unsigned int color) {
	unsigned int i = 0;

	// writes string to video memory
	while(string[i] != '\0') {
		// the characters ascii
		vidptr[current_loc++] = string[i++];
		// attribute byte - give character black bg
		vidptr[current_loc++] = color;
	}
}

void write_newline(void) {
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
	current_loc = current_loc + (line_size - current_loc % (line_size));
}

void key_handlr_main(void) {
	unsigned char status;
	char keycode;
	
	// write EOI
	write_port(0x20, 0x20);

	status = read_port(KEYBOARD_STATUS_PORT);
	// Lowest bit of status will be set if buffer is not empty
	if(status & 0x1) {
		keycode = read_port(KEYBOARD_STATUS_PORT);
		if(keycode < 0) {
			return;
		}

		if(keycode == ENTER_KEY_CODE) {
			write_newline();
			return;
		}

		vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
		vidptr[current_loc++] = 0x07;
	}
}

void kmain(void) {
	const char *str = "[1.11] My Grubby Kernel";
	const char *str2 = "[2.22]  What to do next? :)";

	clear_screen();

	// write first string
	write_string(str, 0x07);
	write_newline();

	// delay and break new line
	delay();

	// write second string
	write_string(str2, 0x03);
	write_string("!~", 0x12);

	delay();

	return;
}

void cmain(void) {
	const char *str = "[+] loading kernel scripts";
	const char *str2 = "[3.33] ... Finished Booting ...";
	
	clear_screen();

	write_string(str, 0x05);
	delay();
	
	write_newline();
	write_string(str2, 0x06);
	delay();

	// start keyboard
	idt_init();
	kb_init();

	while(1);
}
