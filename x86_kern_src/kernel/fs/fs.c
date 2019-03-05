#include "fs.h"
#include "../../grub_framework/headers/s_errno.h"

uint32_t read_fs(fs_node_t *node, uint64_t offset, uint32_t size, uint8_t *buffer)
{
        if(!node)
	{
		return -ENOENT;
	}

	if(node->read)
	{
		uint32_t ret = node->read(node, offset, size, buffer);
		return ret;
	}
	else {
		return -EINVAL;
	}
}

uint32_t write_fs(fs_node_t *node, uint64_t offset, uint32_t size, uint8_t *buffer)
{
	if(!node)
        {
                return -ENOENT;
        }

        if(node->write)
        {
                uint32_t ret = node->write(node, offset, size, buffer);
                return ret;
        }
        else {
                return -EROFS;
        }
}
