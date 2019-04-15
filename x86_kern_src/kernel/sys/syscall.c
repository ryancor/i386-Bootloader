#include "syscall_nums.h"
#include "../../headers/screen.h"
#include "../../headers/system.h"

static process_t *current_process = {0};

static int __attribute__((noreturn)) sys_exit(int retval)
{
	task_exit((retval & 0xFF) << 8);
	for(;;) ;
}

static int sys_geteuid(void)
{
	return current_process->user;
}

static int sys_open(const char *file, int flags, int mode)
{
	int fd = 0;
	return fd;
}

static int sys_read(int fd, char *ptr, int len)
{
	return fd;
}

static int sys_write(int fd, char *ptr, int len)
{
	return fd;
}

static int sys_close(int fd)
{
	return 0;
}

static int (*syscalls[]) = 
{
	// System call table
	[SYS_EXT] = sys_exit,
	[SYS_GETEUID] = sys_geteuid,
	[SYS_OPEN] = sys_open,
	[SYS_READ] = sys_read,
	[SYS_WRITE] = sys_write,
	[SYS_CLOSE] = sys_close,
};

uint32_t num_syscalls = sizeof(syscalls) / sizeof(*syscalls);

void syscalls_install(void)
{
	char syscall_ascii[2] = "";
	int_to_ascii(num_syscalls, syscall_ascii);
	kprint("\n[+] Initializing syscall table with ", 0x07);
	kprint(syscall_ascii, 0x08);
	kprint(" functions\n", 0x07);
}
