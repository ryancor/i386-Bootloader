#ifndef TIME_H
#define TIME_H

#include "../grub_framework/headers/s_stdint.h"

#define NULL	((void*)0)

struct timeval
{
	uint32_t tv_sec;
	uint32_t tv_usec;
};

uint32_t epoch_now();

#endif
