#include <stdint.h>

size_t strlen(const char *s);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void* s, int c, size_t n);
int strcmp(const char *s1, const char *s2);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
void tolowercase(char *in);
void reverse(char s[]);
void int_to_ascii(int n, char str[]);
void append(char s[], char n);
void strappend(char *dest, const char *n);
void strsplit(char *orig, char separator, char *a, char *b);