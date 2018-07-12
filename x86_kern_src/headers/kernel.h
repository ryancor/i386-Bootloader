#define LINES				25
#define BYTES_FOR_EACH_ELEMENT 		2
#define COLUMNS_IN_LINE			80
#define SCREENSIZE			BYTES_FOR_EACH_ELEMENT * COLUMN_IN_LINE * LINES

#define KEYBOARD_DATA_PORT		0x60
#define KEYBOARD_STATUS_PORT		0x64
#define IDT_SIZE			256
#define INTERRUPT_GATE			0x8e
#define KERNEL_CODE_SEG_OFFSET		0x08

#define ENTER_KEY_CODE			0x1c


unsigned int current_loc = 0; // current cursor location
char *vidptr = (char*)0xb8000; // video memory begin


struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};
