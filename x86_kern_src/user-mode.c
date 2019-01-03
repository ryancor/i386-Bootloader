#include "headers/kernel.h"
#include "headers/user-mode.h"
#include "grub_framework/headers/s_string.h"

void terminal_commands(char *keycode_string)
{
	if(strcmp(keycode_string, "ls") == 0)
	{
		kprint("/home    /var    /lib", 0x07);
		kprint_newline();
	}
	else {
		kprint("Command ", 0x07);
		kprint(keycode_string, 0x08);
		kprint(" not recognized.", 0x07);
		kprint_newline();
	}
}
