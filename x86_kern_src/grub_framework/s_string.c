#include "headers/s_string.h"

void append(char s[], char n)
{
	int len = strlen(s);
	s[len] = n;
	s[len+1] = '\0';
}

void backspace(char s[])
{
	int len = strlen(s);
	s[len-1] = '\0';
}

void flush(char *var) 
{
	int i = 0;
	while(var[i] != '\0') 
	{
		var[i] = '\0';
		i++;
	}
}

void *memcpy(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;
	
	for(size_t i = 0; i < n; i++)
	{
		d[i] = s[i];
	}
	return dest;
}

void *memset(void *str, int c, size_t n)
{
	char *s = str;

	for(size_t i = 0; i < n; i++)
	{
		*(s + i) = c;
	}

	return str;
}

void memory_copy(unsigned char *source, unsigned char *dest, int nbytes) 
{
	int i;
	for (i = 0; i < nbytes; i++) 
	{
		*(dest + i) = *(source + i);
	}
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len)
{
	uint8_t *temp = (uint8_t *)dest;
	for( ; len != 0; len--)
	{
		 *temp++ = val;
	}
}

int strcmp(const char *stra, const char *strb)
{
	unsigned int i;

	for(i = 0; stra[i] == strb[i]; i++)
	{
		if(stra[i] == '\0')
		{
			// string match
			return 0;
		}
	}
	return stra[i] - strb[i];
}

size_t strlen(const char *str)
{
	size_t length = 0;
	while(str[length])
	{
		length++;
	}
	return length;
}

size_t strnlen(const char *s, size_t maxlen)
{
	size_t len;

	for(len = 0; len < maxlen; len++, s++)
	{
		if(!*s)
		{
			break;
		}
	}
	return (len);
}

char *strcat(char *dest, const char *src)
{
	unsigned int dest_size = strlen(dest);
	unsigned int src_size = strlen(src);

	char *d = dest;

	for(size_t i = 0; i < src_size; i++)
	{
		d[dest_size + i] = src[i];
	}

	d[dest_size + src_size] = '\0';

	return dest;
}

char *s_strncpy(char *s1, const char *s2, size_t n)
{
	size_t size = strnlen(s2, n);
	if(size != n)
	{
		memset(s1 + size, '\0', n - size);
	}	
	return memcpy(s1, s2, size);
}

void int_to_ascii(int n, char str[])
{
	int i, sign;
	if((sign = n) < 0)
	{
		n = -n;
	}
	
	i = 0;
	do
	{
		str[i++] = n % 10 + '0';
	} while((n /= 10) > 0);

	if(sign < 0)
	{
		str[i++] = '-';
	}
	str[i] = '\0';
}

void hex_to_ascii(int n, char str[])
{
	append(str, '0');
	append(str, 'x');
	char zeros = 0;

	int32_t tmp;
	int i;
	for(i = 28; i > 0; i -= 4)
	{
		tmp = (n >> i) & 0xF;
		if(tmp == 0 && zeros == 0)
		{
			continue;
		}
		zeros = 1;
		if(tmp > 0xA) 
		{
			append(str, tmp - 0xA + 'a');
		}
		else {
			append(str, tmp + '0');
		}
	}

	tmp = n & 0xF;
	if(tmp >= 0xA)
	{
		append(str, tmp - 0xA + 'a');
	}
	else {
		append(str, tmp + '0');
	}
}

// These functions need to be built out
size_t s_vasprintf(char *buf, const char *fmt)//, va_list args)
{
	return 1;
}

int s_sprintf(char *buf, const char *fmt)
{
	//va_list args;
	//va_start(args, fmt);
	int out = s_vasprintf(buf, fmt);//, args);
	//va_end(args);
	return out;
}

void *s_malloc(unsigned int size)
{
	void *ret = (void*)size;
	return ret;
}

uint32_t free_mem_addr = 0x10000;

uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr)
{
	// Pages are aligned to 4K, or 0x1000
	if(align == 1 && (free_mem_addr & 0xFFFFF000))
	{
		free_mem_addr &= 0xFFFFF000;
		free_mem_addr += 0x1000;
	}
	// Save the physical address
	if(phys_addr)
	{
		*phys_addr = free_mem_addr;
	}

	uint32_t ret = free_mem_addr;
	free_mem_addr += size; 
	return ret;
}

