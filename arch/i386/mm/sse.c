#include <stdint.h>

uint8_t has_sse = 0;
char sse_enabled = 0;
uint8_t detect_sse()
{
	has_sse = _detect_sse();
	return has_sse;
}

void setup_sse()
{
	if (_detect_sse() == 1)
	{
		_enable_sse();
		sse_enabled = 1;
		printkok("Enabled SSE");
	} else {
		printkfail("SSE not available on this CPU");
	}
}

void sse_memcpy(void *dest, const void *src, size_t n)
{
	if (has_sse == 1)
	{
		n = (n + 127) / 128;
		_sse_memcpy(dest, src, n);
	}
}