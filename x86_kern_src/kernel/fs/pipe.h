#include "fs.h"
#include "list.h"

typedef struct _pipe_device
{
	uint8_t *buffer;
	size_t write_ptr;
	size_t read_ptr;
	size_t size;
	size_t refcount;
	volatile int lock_read[2];
	volatile int lock_write[2];
	list_t *wait_queue_readers;
	list_t *wait_queue_writers;
	int dead;
	list_t *alert_waits;
} pipe_device_t;

typedef struct 
{
        list_t *node_waits;
} process_t;


fs_node_t *make_pipe(size_t size);
int pipe_size(fs_node_t *node);
