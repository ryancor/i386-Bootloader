#include "../grub_framework/headers/s_stdint.h"
#include "../grub_framework/headers/s_string.h"

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

void *end;
void paging_install(uint32_t memsize);
void heap_install(void);
