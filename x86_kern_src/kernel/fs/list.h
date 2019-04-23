#include "fs.h"

typedef struct node 
{
	struct node * next;
	struct node * prev;
	void * value;
	void * owner;
} __attribute__((packed)) node_t;

typedef struct 
{
	node_t * head;
	node_t * tail;
	size_t length;
} __attribute__((packed)) list_t;

#define foreach(item, list)	for(node_t *(item) = (node_t*)(list); (item); (item) = (item)->next)

void list_append(list_t *list, node_t *item);
node_t *list_insert(list_t *list, void *item);
list_t *list_create(void);
node_t *list_find(list_t *list, void *value);
