#include <string.h>
#include <stdint.h>

/* Simple function to get a string's length */
// UNIX Compatible
size_t strlen(const char *s)
{
  if (!s)
  {
    return 0;
  }
  
  const char *ptr = s;

  while (*s)
  {
    ++s;
  }

  return s - ptr;
}

/* Simple function to copy a memory block from source to destination */
// UNIX Compatible
void *memcpy(void *dest, const void *src, size_t n)
{
  uint8_t *d = dest;
  const uint8_t *s = src;

  while(n--)
  {
    *d++ = *s++;
  }

  return dest;
}

/* Simple function to copies characters to a defined location */
// UNIX Compatible  
void *memset(void* s, int c, size_t n)
{
  uint8_t* p = (uint8_t*)s;
  uint8_t x = c & 0xff;

  while (n--)
  {
    *p++ = x;
  }

  return s;
}

/* Simple function to compare two C strings */
// UNIX Compatible
int strcmp(const char *s1, const char *s2)
{
  int i = 0;

  while(s1[i] == s2[i] && s1[i] != '\0' && s2[0] != '\0')
  {
    i++;
  }

  if (s1[i] == '\0' && s2[i] == '\0')
  {
    return 0;
  }

  return 1;
}

/* Function to copy a string */
// UNIX Compatible
char *strcpy(char *dest, const char *src)
{
  char *p = dest;

  while (*src)
    *p++ = *src++;
  *p = '\0';

  return dest;
}

/* Function to copy 'n' bytes from a string */
// UNIX Compatible
char *strncpy(char *dest, const char *src, size_t n)
{
  size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++)
    dest[i] = src[i];
  
  for ( ; i < n; i++)
    dest[i] = '\0';

   return dest;
}

void memory_zero(void *p, size_t size)
{
  char *p_current = (char*)p;
  char *p_end = p_current + size;

  do
  {
    *p_current = 0;
  } while(++p_current != p_end);
}

int hex8(uint8_t value, char *buf)
{
  const char *hex = "0123456789ABCDEF";
  
  buf[0] = hex[(value >> 4)];
  buf[1] = hex[value & 0xF];
  buf[2] = '\0';
}

int hex16(uint16_t value, char *buf)
{
  const char *hex = "0123456789ABCDEF";
  
  buf[0] = hex[(value >> 12) & 0xF];
  buf[1] = hex[(value >> 8) & 0xF];
  buf[2] = hex[(value >> 4) & 0xF];
  buf[3] = hex[value & 0xF];
  buf[4] = '\0';
}

void hex_dump(void *data, size_t len)
{
  const int bytes_per_row = 16;
  uint8_t *ptr = (uint8_t*)data;
  char hex_buffer[5];
  size_t rows = len / bytes_per_row;
  uint16_t row = 0;

  for (int i = 0; i < len; ++i)
  {
    if ((i % bytes_per_row) == 0)
    {
      if(i != 0)
      {
        printk("\n");
      }

      hex16(row * bytes_per_row, hex_buffer);
      printk(hex_buffer);
      printk(" ");
      row++;
    }
    
    hex8(ptr[i], hex_buffer);
    printk(hex_buffer);
    printk(" ");  
  }
}