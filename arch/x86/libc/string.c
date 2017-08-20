#include <stdint.h>

/* Simple function to get a string's length */
// UNIX Compatible
size_t strlen(const char *s) {
  if (!s) {
    return 0;
  }
  char *ptr = s;
  while (*s) {
    ++s;
  }
  return s - ptr;
}

/* Simple function to copy a memory block from source to destination */
// UNIX Compatible
void *memcpy(void *dest, const void *src, size_t n) {
  uint8_t *d = dest;
  const uint8_t *s = src;
  while(n--) {
    *d++ = *s++;
  }
  return dest;
}

/* Simple function to copies characters to a defined location */
// UNIX Compatible  
void *memset(void* s, int c, size_t n) {
  uint8_t* p = (uint8_t*)s;
  uint8_t x = c & 0xff;
  while (n--) {
    *p++ = x;
  }
  return s;
}