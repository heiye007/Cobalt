#include <stdint.h>

#define PANIC(err) x86_panic(err);

void x86_panic(const char* err);