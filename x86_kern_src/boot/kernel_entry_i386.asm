;; kernel
[bits 32]

global start
global keyboard_handler
global read_port
global write_port
global load_idt

extern cmain		; defined in c file
extern kmain 		; this is defined in the c file
extern keyboard_handler_main ; defined in the kernel.c

read_port:
	mov   edx, [esp + 4]
	in    al, dx	; dx is the lower 16 bits of edx
	ret

write_port:
	mov   edx, [esp + 4]
	mov   al, [esp + 4 + 4]
	out   dx, al
	ret

load_idt:
	mov   edx, [esp + 4]
	lidt  [edx]
	sti 				; turn on interrupts
	ret

keyboard_handler:
	call  keyboard_handler_main
	iretd

start:
	call  cmain
	call  kmain
	jmp   $
