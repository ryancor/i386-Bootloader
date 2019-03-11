#include "../headers/time.h"
#include "../headers/ports.h"
#include "../grub_framework/headers/s_stdint.h"

#define from_bcd(val)	((val / 16) * 10 + (val & 0xf))

void cmos_dump(uint16_t *values)
{
	uint16_t index;
	for(index = 0; index < 128; index++)
	{
		port_byte_out(0x70, index);
		values[index] = port_byte_in(0x71);
	}
}

void get_date(uint16_t *month, uint16_t *day)
{
	uint16_t values[128]; 
	cmos_dump(values);

	*month = from_bcd(values[8]);
	*day = from_bcd(values[7]);
}

void get_time(uint16_t *hours, uint16_t *minutes, uint16_t *seconds)
{
	uint16_t values[128];
	cmos_dump(values);

	*hours = from_bcd(values[4]);
	*minutes = from_bcd(values[2]);
	*seconds = from_bcd(values[0]);
}

uint32_t secs_of_years(int years)
{
	uint32_t days = 0;
	years += 2000;
	while(years > 1969)
	{
		days += 365;
		if(years % 4 == 0)
		{
			if(years % 100 == 0)
			{
				if(years % 400 == 0)
				{
					days++;
				}
			}
			else {
				days++;
			}
		}
		years--;
	}
	return days * 86400;
}

uint32_t secs_of_month(int months, int year)
{
	year += 2000;

	uint32_t days = 0;
	switch(months)
	{
		case 11:
			days += 30;
		case 10:
			days += 31;
		case 9:
			days += 30;
		case 8:
			days += 31;
		case 7:
			days += 31;
		case 6:
			days += 30;
		case 5:
			days += 31;
		case 4:
			days += 30;
		case 3:
			days += 31;
		case 2:
			days += 28;
			if((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0)))
			{
				days++;
			}
		case 1:
			days += 31;
		default:
			break;
	}
	return days * 86400;
}

int gettimeofday(struct timeval *t, void *z)
{
	uint16_t values[128];
	cmos_dump(values);

	uint32_t time = secs_of_years(from_bcd(values[9]) - 1) +
		secs_of_month(from_bcd(values[8]) - 1, from_bcd(values[9])) +
		(from_bcd(values[7]) - 1) * 86400 +
		(from_bcd(values[4])) * 3600 +
		(from_bcd(values[2])) * 60 +
		from_bcd(values[0]) +
		0;

	t->tv_sec = time;
	t->tv_usec = 0;
	return 0;
}

uint32_t epoch_now()
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec;
}
