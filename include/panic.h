#include <stdint.h>

extern void panic(char* err, const char *file, uint32_t line);
#define PANIC(err) panic(err, __FILE__, __LINE__);