#define LINES 				25
#define COLUMNS_IN_LINE 		80
#define BYTES_FOR_EACH_ELEMENT 		2
#define SCREENSIZE 			BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define KEYBOARD_DATA_PORT 		0x60
#define KEYBOARD_STATUS_PORT 		0x64
#define IDT_SIZE 			256
#define INTERRUPT_GATE 			0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 	0x08

#define ENTER_KEY_CODE 			0x1C
#define ENTER_BACKSPACE_CODE		0x0E

// Screen I/O ports
#define REG_SCREEN_CTRL			0x3d4
#define REG_SCREEN_DATA			0x3d5

static unsigned int current_loc = 0; // current cursor location
static char *vidptr = (char*)0xb8000; // video memory begin


struct IDT_entry 
{
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

// API for print to screen
void kprint(const char *string, unsigned int color);
void kprint_newline(void);
