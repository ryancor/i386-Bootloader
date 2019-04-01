#include "../../headers/system.h"

void initialize_process_tree(void)
{
	return;
}

process_t *spawn_init(void)
{
	process_t *init = s_malloc(sizeof(process_t));
	return init;
}

process_t *spawn_kidle(void)
{
	process_t *idle = s_malloc(sizeof(process_t));
	memset(idle, 0x00, sizeof(process_t));
	return idle;
}
