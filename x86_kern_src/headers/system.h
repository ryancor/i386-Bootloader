#include "time.h"
#include "../grub_framework/headers/s_stdint.h"
#include "../grub_framework/headers/s_string.h"
#include "../kernel/interrupts/headers/int.h"

typedef signed int pid_t;
typedef unsigned int user_t;
typedef unsigned int status_t;

#define KERNEL_STACK_SIZE	0x8000
#define NUMSIGNALS		38
#define USER_ROOT_UID		(user_t)0
#define IRQ_OFF			{ asm volatile("cli"); }
#define IRQ_RES			irqrestore(1)
#define IRQ_ON			irqrestore(2)
#define PAUSE			{ asm volatile("hlt"); }

typedef struct page 
{
	unsigned int present:1;
	unsigned int rw:1;
	unsigned int user:1;
	unsigned int writethrough:1;
	unsigned int cachedisable:1;
	unsigned int accessed:1;
	unsigned int dirty:1;
	unsigned int pat:1;
	unsigned int global:1;
	unsigned int unused:3;
	unsigned int frame:20;
} __attribute__((packed)) page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	uintptr_t physical_tables[1024];	// Physical address of the tables
	page_table_t *tables[1024];		// 1024 pointers to page table
	uintptr_t physical_address;		// Physical address of physical_tables
	int32_t ref_count;
} page_directory_t;

typedef struct thread
{
	uintptr_t esp;
	uintptr_t ebp;
	uintptr_t eip;
	
	uint8_t fpu_enabled;
	uint8_t fp_regs[512];
	
	uint8_t padding[32];
	
	page_directory_t *page_directory;
} thread_t;

typedef struct image
{
	size_t size;
	uintptr_t entry;
	uintptr_t heap;
	uintptr_t heap_actual;
	uintptr_t stack;
	uintptr_t user_stack;
	uintptr_t start;
	uintptr_t shm_heap;
	volatile int lock[2];
} image_t;

typedef struct descriptor_table
{
	//fs_node_t **entries;
	uint64_t *offsets;
	int *modes;
	size_t length;
	size_t capacity;
	size_t refs;
} fd_table_t;

typedef struct signal_table
{
	uintptr_t functions[NUMSIGNALS+1];
} sig_table_t;

typedef struct process
{
	pid_t id;
	char *name;
	char *description;
	user_t user;
	user_t real_user;
	int mask;

	char **cmdline;

	pid_t group;
	pid_t job;
	pid_t session;

	thread_t thread;
	//tree_node_t *tree_entry;
	image_t image;
	//fs_node_t *wd_node;
	char *wd_name;
	fd_table_t *fds;
	status_t status;
	sig_table_t signals;
	uint8_t finished;
	uint8_t started;
	uint8_t running;
	//struct regs *syscall_registers;
	//list_t *wait_queue;
	//list_t *shm_mappings;
	//list_t *signal_queue;
	thread_t signal_state;
	char *signal_kstack;
	//node_t sched_node;
	//node_t sleep_node;
	//node_t *timed_sleep_node;
	uint8_t is_tasklet;
	volatile uint8_t sleep_interrupted;
	//list_t *node_waits;
	int awoken_index;
	//node_t *timeout_node;
	struct timeval start;
	uint8_t suspended;
} process_t;

void *end;
void paging_install(uint32_t memsize);
void heap_install(void);

void initialize_process_tree(void);
process_t *spawn_init(void);
process_t *spawn_kidle(void);
void switch_page_directory(page_directory_t *dir);
void tasking_install(void);
void task_exit(int retval);

void syscalls_install(void);
