#include <i386/system.h>

uint8_t inb (unsigned short _port)
{
    uint8_t rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outb (unsigned short _port, uint8_t _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void outw(uint16_t _data, uint16_t _port)
{
	__asm__ __volatile__ ("outw %w0, %w1" : : "a" (_data), "Nd" (_port));
}

uint16_t inw(uint16_t _port)
{
	uint16_t _data;
	__asm__ __volatile__ ("inw %w1, %w0" : "=a"(_data) : "Nd" (_port));
	return _data;
}