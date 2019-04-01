#include "../../headers/screen.h"
#include "../../headers/system.h"

process_t *current_process;
process_t *kernel_idle_task;
page_directory_t *kernel_directory;
page_directory_t *current_directory;

uintptr_t frozen_stack = 0;

void tasking_install(void)
{
	IRQ_OFF; // Disable interrupts

	kprint("\n[!] Initializing Multi-tasking\n", 0x07);

	// Initialize the process tree
	initialize_process_tree();
	// spawn the initial process
	current_process = spawn_init();
	kernel_idle_task = spawn_kidle();
	// Initialize the paging environment
#if 0
	set_process_environment((process_t*)current_process, current_directory);
#endif
	// Switch to the kernel directory
	switch_page_directory(current_process->thread.page_directory);

	frozen_stack = (uintptr_t)kmalloc(KERNEL_STACK_SIZE, 0, (uint32_t*)&current_process);

	// Re-enable interrupts
	IRQ_RES;
	kprint("[+] Multi-tasking finished initializing!\n", 0x07);
}
