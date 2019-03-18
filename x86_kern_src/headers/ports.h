#include "../grub_framework/headers/s_stdint.h"

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned int port, unsigned char data);
unsigned short port_word_in(uint16_t port);
void port_word_out(uint16_t port, uint16_t data);
void APMInterface_Init();
