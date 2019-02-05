#include "headers/s_string.h"

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
