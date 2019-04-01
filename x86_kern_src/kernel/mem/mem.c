#include "../../headers/system.h"
#include "../../headers/kernel.h"

#define INDEX_FROM_BIT(b)	(b / 0x20)
#define OFFSET_FROM_BIT(b)	(b % 0x20)

extern void *end;
uint32_t *frames;
uint32_t nframes;
uintptr_t placement_pointer = (uintptr_t)&end;
uintptr_t heap_end = (uintptr_t)NULL;
page_directory_t *current_directory;
page_directory_t *kernel_directory;

void paging_install(uint32_t memsize)
{
	uintptr_t phys;

	memsize -= 0xe001e190; // subtract from multiboot page to prevent overflowing
	nframes = memsize / 4;
	frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes*8), 0, (uint32_t*)&phys);
	memset(frames, 0, INDEX_FROM_BIT(nframes * 8));

	kernel_directory = (page_directory_t *)kmalloc(0, sizeof(page_directory_t), (uint32_t*)&phys);
	memset(kernel_directory, 0, sizeof(page_directory_t));

	asm volatile (
		"mov $0x277, %%ecx\n"	    // IA32_MSR_PAT
		"rdmsr\n"
		"or $0x1000000, %%edx\n"   // set bit 56
		"and $0xf9ffffff, %%edx\n" // unset bits 57, 58
		"wrmsr\n"
		: : : "ecx", "edx", "eax"
	);
}

void heap_install(void)
{
	heap_end = (placement_pointer + 0x1000) & ~0xFFFF;
}

void switch_page_directory(page_directory_t *dir)
{
	current_directory = dir;
	asm volatile(
		"mov %0, %%cr3\n"
		"mov %%cr0, %%eax\n"
		"orl $0x80000000, %%eax\n"
		"mov %%eax, %%cr3\n"
		:: "r"(dir->physical_address)
		: "%eax"
	);
}

