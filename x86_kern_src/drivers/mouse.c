#include "../cpu/isr.h"
#include "../kernel/fs/fs.h"
#include "../kernel/fs/pipe.h"
#include "../headers/ports.h"
#include "../headers/mouse.h"
#include "../headers/screen.h"

#define PACKETS_IN_PIPE 1024
#define DISCARD_POINT 	32

#define MOUSE_IRQ 	12

#define MOUSE_PORT   	0x60
#define MOUSE_STATUS 	0x64
#define MOUSE_ABIT   	0x02
#define MOUSE_BBIT   	0x01
#define MOUSE_WRITE  	0xD4
#define MOUSE_F_BIT  	0x20
#define MOUSE_V_BIT  	0x08

unsigned char mouse_cycle = 0;
signed char mouse_byte[3];
signed char mouse_x = 0;
signed char mouse_y = 0;

fs_node_t *mouse_pipe;

void mouse_handler(isr_t *a_r)
{
	switch(mouse_cycle)
	{
		case 0:
			mouse_byte[0] = port_byte_in(MOUSE_PORT);
			mouse_cycle++;
			break;
		case 1:
			mouse_byte[1] = port_byte_in(MOUSE_PORT);
			mouse_cycle++;
			break;
		case 2:
			mouse_byte[2] = port_byte_in(MOUSE_PORT);
			if(mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40)
			{
				break;
			}

			mouse_device_packet_t packet;
			packet.magic = MOUSE_MAGIC;
			packet.x_difference = mouse_byte[1];
			packet.y_difference = mouse_byte[2];
			packet.buttons = 0;
			if(mouse_byte[0] & 0x01)
			{
				packet.buttons |= LEFT_CLICK;
			}
			if(mouse_byte[0] & 0x02)
			{
				packet.buttons |= RIGHT_CLICK;
			}
			if(mouse_byte[0] & 0x04)
			{
				packet.buttons |= MIDDLE_CLICK;
			}

			mouse_x = mouse_byte[1];
			mouse_y = mouse_byte[2];
			mouse_cycle = 0;

			mouse_device_packet_t bitbucket;
			// have to write these functions			
			while(pipe_size(mouse_pipe) > (DISCARD_POINT * sizeof(packet)))
			{
				read_fs(mouse_pipe, 0, sizeof(packet), (uint8_t *)&packet);
			}
			write_fs(mouse_pipe, 0, sizeof(packet), (uint8_t *)&packet);
			break;
	}
}

void mouse_wait(unsigned char a_type)
{
	unsigned int _time_out = 100000;
	if(a_type == 0)
	{
		while(_time_out--)
		{
			if((port_byte_in(MOUSE_STATUS) & 1) == 1)
			{
				return;
			}
		}
		return;
	}
	else {
		while(_time_out--)
		{
			if((port_byte_in(MOUSE_STATUS) & 2) == 0)
			{
				return;
			}
		}
		return;
	}
}

void mouse_write(unsigned char a_write)
{
	// Wait to be able to send a command
	mouse_wait(1);
	// tell the mouse we are sending a command
	port_byte_out(MOUSE_STATUS, MOUSE_WRITE);
	// Wait for the final part
	mouse_wait(1);
	// Finally write
	port_byte_out(MOUSE_PORT, a_write);
}

unsigned char mouse_read()
{
	// Get a response from mouse
	mouse_wait(0);
	return port_byte_in(MOUSE_PORT);
}

void mouse_install()
{
	unsigned char _status;

	mouse_pipe = make_pipe(sizeof(mouse_device_packet_t) * PACKETS_IN_PIPE);

	// Enable the aux mouse device
	mouse_wait(1);
	port_byte_out(MOUSE_STATUS, 0xA8);

	// enable the interrupts
	mouse_wait(1);
	port_byte_out(MOUSE_STATUS, MOUSE_F_BIT);
	mouse_wait(0);
	_status = (port_byte_in(MOUSE_PORT) | 2);
	mouse_wait(1);
	port_byte_out(MOUSE_STATUS, MOUSE_PORT);
	mouse_wait(1);
	port_byte_out(MOUSE_PORT, _status);

	// Tell the mouse to use default settings
	mouse_write(0xF6);
	mouse_read();

	// Enable the mouse
	mouse_write(0xF4);
	mouse_read();

	// Setup the mouse handler
	register_interrupt_handler(MOUSE_IRQ, (isr_t)mouse_handler);

	kprint("\n[+] Mouse Driver Initialized\n", 0x07);
}
