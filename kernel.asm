;; kernel
bits 32
section .text
	;multiboot spec
	align 4
	dd 0x1BADB002	; magic
	dd 0x00		; flags
	dd - (0x1BADB002 - 0x00) ; checksum. m+f+c should be zero


global start
extern kmain		; kmain is defined in c file
extern cmain		; cmain is defined in c file

start:
	cli		     ; blocks interrupts
	mov   esp, stack_space ; set stack pointer
	call  kmain

	mov   ecx, nl	     ; new line
	mov   edx, 0	     ; new line

	call  cmain
	cmp   edx, 0	     ; if edx is 0 jmp to exit (loop)
	je   _exit
	hlt		     ; halt the CPU

	
_exit:
	mov   ebx, 0
	mov   eax, 1	     ; sys_call for exit
	int   0x80

section .bss
resb 8192		; 8kb for stack
stack_space:

section .data
	nl db ' ', 0x0A
