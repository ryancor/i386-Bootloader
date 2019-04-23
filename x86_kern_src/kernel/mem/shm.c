#include "../../headers/screen.h"
#include "../../headers/system.h"

tree_t *shm_tree = NULL;

tree_t *tree_create(void)
{
	tree_t *out = kmalloc(sizeof(tree_t), 0, (uint32_t*)&out);
	out->nodes = 0;
	out->root = NULL;
	return out;
}

tree_node_t *tree_node_create(void *value)
{
	tree_node_t *out = kmalloc(sizeof(tree_node_t), 0, (uint32_t*)&out);
	out->value = value;
	out->children = list_create();
	out->parent = NULL;
	return out;
}

void tree_set_root(tree_t *tree, void *value)
{
	tree_node_t *root = tree_node_create(value);
	tree->root = root;
	tree->nodes = 1;
}

void shm_install(void)
{
	kprint("[+] Installing shared memory layer...\n", 0x07);
	shm_tree = tree_create();
	tree_set_root(shm_tree, NULL);	
}
