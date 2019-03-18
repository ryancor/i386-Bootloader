#include "../headers/time.h"
#include "../headers/ports.h"
#include "../headers/kernel.h"
#include "../headers/screen.h"
#include "../headers/shell.h"
#include "../kernel/interrupts/headers/int.h"
#include "../grub_framework/headers/s_string.h"

void terminal_commands(char *keycode_string)
{
	if(strcmp(keycode_string, "ls") == 0)
	{
		kprint("/home    /var    /lib", 0x07);
		kprint_newline();
	}
	else if(strcmp(keycode_string, "pwd") == 0) {
		kprint("/", 0x07);
		kprint_newline();
	}
	else if(strcmp(keycode_string, "clear") == 0) {
		clear_screen();
	}
	else if(strcmp(keycode_string, "restart") == 0) {
		kprint("Resetting...", 0x07);
		delay();
		APMInterface_Init();
	}
	else if(strcmp(keycode_string, "shutdown") == 0) {
		kprint("Goodbye!", 0x07);
		// disable interrupts (if enabled), then halt the CPU
		if(are_interrupts_enabled())
		{
			__asm__("cli");
		}
		__asm__("hlt");
	}
	else if(strcmp(keycode_string, "whoami") == 0) {
		kprint("Ringo, but lets call you root ;)", 0x07);
		kprint_newline();
	}
	else if(strcmp(keycode_string, "time") == 0) {
		char time_ascii[256];
		int_to_ascii(epoch_now(), time_ascii);

		kprint("Current time: ", 0x07);
		kprint(time_ascii, 0x07);
		kprint_newline();
	}
	else if(strcmp(keycode_string, "page") == 0) {
		uint32_t phys_addr;
		uint32_t page = kmalloc(1000, 1, &phys_addr);
		char page_str[16] = "";
		hex_to_ascii(page, page_str);
		char phys_str[16] = "";
		hex_to_ascii(phys_addr, phys_str);
		kprint("Page: ", 0x07);
		kprint(page_str, 0x07);
		kprint(", physical address: ", 0x07);
		kprint(phys_str, 0x07);
		kprint("\n", 0x07);		
	}
	else {
		kprint("Command ", 0x07);
		kprint(keycode_string, 0x08);
		kprint(" not recognized.", 0x07);
		kprint_newline();
	}
}
