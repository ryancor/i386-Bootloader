#include "../headers/ports.h"
#include "../headers/kernel.h"
#include "../grub_framework/headers/s_string.h"

int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
int get_cursor_offset();
void set_cursor_offset(int offset);

int print_char(char c, int col, int row, unsigned int color)
{
	unsigned char *vidmem = (unsigned char*) vidptr;

	if (col >= COLUMNS_IN_LINE || row >= LINES)
	{
		vidmem[2*(COLUMNS_IN_LINE)*(LINES)-2] = 'E';
		vidmem[2*(COLUMNS_IN_LINE)*(LINES)-1] = color;
		return get_offset(col, row);
	}

	int offset;
	if (col >= 0 && row >= 0)
	{
		offset = get_offset(col, row);
	}
	else {
		offset = get_cursor_offset();
	}

	if (c == '\n')
	{
		row = get_offset_row(offset);
		offset = get_offset(0, row+1);
	}
	else if (c == 0x08) {
		vidmem[offset] = ' ';
		vidmem[offset+1] = color;
	}
	else {
		vidmem[offset] = c;
		vidmem[offset+1] = color;
		offset += 2;
	}

	// Check if the offset is over screen size and scroll
	if (offset >= LINES * COLUMNS_IN_LINE * 2)
	{
        	int i;
		for (i = 1; i < LINES; i++)
		{
			memory_copy((unsigned char*)(get_offset(0, i) + vidptr),
				(unsigned char*)(get_offset(0, i-1) + vidptr),
				COLUMNS_IN_LINE * 2);
		}

		// Blank last line
		char *last_line = (char*) (get_offset(0, LINES-1) + (unsigned char*) vidptr);
		for (i = 0; i < COLUMNS_IN_LINE * 2; i++)
		{
			last_line[i] = 0;
		}

		offset -= 2 * COLUMNS_IN_LINE;
	}

	set_cursor_offset(offset);
	return offset;
}

void kprint_at(const char *message, int col, int row, unsigned int color)
{
	// Set cursor if col/row are negative
	int offset;
	if (col >= 0 && row >= 0)
	{
		offset = get_offset(col, row);
	}
	else {
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}

	// Loop through message and print it
	int i = 0;
	while (message[i] != 0)
	{
		offset = print_char(message[i++], col, row, color);
		// Compute row/col for next iteration
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void kprint(const char *message, unsigned int color)
{
	kprint_at(message, -1, -1, color);
}

void kprint_newline(void)
{
	kprint_at("\n", -1, -1, 0xff);
}

void kprint_backspace(void)
{
	int offset = get_cursor_offset() - 2;
	int row = get_offset_row(offset);
	int col = get_offset_col(offset);
	print_char(0x08, col, row, 0x0f);
}

void clear_screen(void)
{
	unsigned int i = 0;

	// this loop clears the screen
	// each line takes 2 bytes
	while (i < SCREENSIZE)
	{
		// blank character
		vidptr[i++] = ' ';
		// attribute byte
		vidptr[i++] = 0x07;
	}
	set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row)
{
	return 2 * (row * COLUMNS_IN_LINE + col);
}

int get_offset_row(int offset)
{
	return offset / (2 * COLUMNS_IN_LINE);
}

int get_offset_col(int offset)
{
	return (offset - (get_offset_row(offset) * 2 * COLUMNS_IN_LINE)) / 2;
}

int get_cursor_offset()
{
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	return offset * 2;
}

void set_cursor_offset(int offset)
{
	offset /= 2;
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void delay()
{
	for(unsigned int n = 0; n < 500000000; n++);
}
