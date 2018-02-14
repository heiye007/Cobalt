#include <stdint.h>

#define PANIC(err) panic(err);

void panic(const char* err);