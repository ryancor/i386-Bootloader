#define BYTES_FOR_EACH_ELEMENT 		2
#define COLUMNS_IN_LINE			80

unsigned int current_loc = 0; // current cursor location
char *vidptr = (char*)0xb8000; // video memory begin
