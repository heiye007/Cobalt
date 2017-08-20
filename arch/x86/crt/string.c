#include <stdint.h>

int strlen(char *str)
{
  if (!str) {
    return 0;
  }
  
  char *ptr = str;
  while (*str) {
    ++str;
  }

  return str - ptr;
}

void memcpy(void *dest, const void *src, unsigned long n) {
  unsigned char *d = dest;
  const unsigned char *s = src;

  while(n--) *d++ = *s++;

  return dest;
}

void memsetw(void *s, signed int c, unsigned long n)
{
	signed int *p = (signed int*)s;
	while (n--) {
		*p++ = c;
	}
	return s;
}

void* memset(void* s, int c, size_t sz) {
    unsigned char* p = (unsigned char*)s;

    unsigned char x = c & 0xff;

    while (sz--)
        *p++ = x;
    return s;
}