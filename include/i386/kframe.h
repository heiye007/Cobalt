#ifndef KFRAME_H
#define KFRAME_H

#include <stdint.h>

void init_frame_allocator(uint32_t system_frames);
void alloc_frame(page *page, int is_kernel, int is_writeable);
void free_frame(page *page);

#endif