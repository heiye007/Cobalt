#include <multiboot.h>

static unsigned char* frames_bitmap;

static unsigned int bitmap_size;

#define FRAME_INDEX(f) (f / 8)
#define FRAME_OFFSET(f) (f % 8)
#define FRAME_NUMBER(i, o) (8 * i + o)

static unsigned int count_memory(multiboot_info_t *multiboot_info)
{
    if (multiboot_info->flags & 0x01)
    {
        unsigned int start = multiboot_info->mem_lower;
        unsigned int end = multiboot_info->mem_upper + 1024;
        return end - start;
    } else {
        return 0;
    }
}

void frame_set(unsigned int frame_no)
{
    unsigned int index = FRAME_INDEX(frame_no);
    unsigned int offset = FRAME_OFFSET(frame_no);
    frames_bitmap[index] |= (1 << offset);
}

void frame_free(unsigned int frame_no)
{
    unsigned int index = FRAME_INDEX(frame_no);
    unsigned int offset = FRAME_OFFSET(frame_no);
    frames_bitmap[index] &= ~(1 << offset);
}

int frame_is_set(unsigned int frame_no)
{
    unsigned int index = FRAME_INDEX(frame_no);
    unsigned int offset = FRAME_OFFSET(frame_no);
    return (frames_bitmap[index] & (1 << offset)) != 0;
}

int frame_first_free()
{
    int index, offset;
    for (index = 0; index < bitmap_size; index++)
    {
        for (offset = 0; offset < 8; offset++) {
            if (!(frames_bitmap[index] & (1 << offset)))
            {
                return FRAME_NUMBER(index, offset);
            }
        }
    }
    return -1;
}

void frame_init(multiboot_info_t* multiboot_ptr)
{
	unsigned int memory_size = count_memory(multiboot_ptr);
	unsigned int frames = memory_size / 4;

	bitmap_size = frames / 8;
	frames_bitmap = kmalloc(bitmap_size);
}