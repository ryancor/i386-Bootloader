#include "s_stdint.h"

#define size_t		unsigned long int

void backspace(char s[]);

void flush(char *var);

void *memcpy(void *dest, const void *src, size_t n);

void *memset(void *str, int c, size_t n);

void memory_copy(unsigned char *source, unsigned char *dest, int nbytes);

void memory_set(uint8_t *dest, uint8_t val, uint32_t len);

int strcmp(const char *stra, const char *strb);

size_t strlen(const char *str);

size_t strnlen(const char *s, size_t maxlen);

char *strcat(char *dest, const char *src);

char *s_strncpy(char *s1, const char *s2, size_t n);

void int_to_ascii(int n, char str[]);

int s_sprintf(char *buf, const char *fmt);

void *s_malloc(unsigned int size);
