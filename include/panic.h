#include <stdint.h>

#define PANIC(err) panic(err, __FILE__, __LINE__);

extern void panic(char* err, const char *file, uint32_t line);