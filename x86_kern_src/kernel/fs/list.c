#include "list.h"

#define NULL	((void*)0)

void list_append(list_t *list, node_t *node)
{
	node->next = NULL;

	node->owner = list;
	if(!list->length)
	{
		list->head = node;
		list->tail = node;
		node->prev = NULL;
		node->next = NULL;
		list->length++;
		return;
	}
	list->tail->next = node;
	node->prev = list->tail;
	list->tail = node;
	list->length++;
}

node_t *list_insert(list_t *list, void *item)
{
	node_t *node = s_malloc(sizeof(node_t));
	node->value = item;
	node->next = NULL;
	node->prev = NULL;
	node->owner = NULL;
	list_append(list, node);

	return node;
}

node_t *list_find(list_t *list, void *value)
{
	
	foreach(item, list)
	{
		if(item->value == value)
		{
			return item;
		}
	}
	return NULL;
}

list_t *list_create(void)
{
	list_t *out = kmalloc(sizeof(list_t), 0, (uint32_t*)&out);
	out->head = NULL;
	out->tail = NULL;
	out->length = 0;
	return out;
}
