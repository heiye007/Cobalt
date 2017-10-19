#define NULL 0
#define MEM_BLOCK_SIZE sizeof(struct memory_block)

#define kzero_memory(addr, size) (kmemset((addr), 0, (size)))

struct memory_block {
    char free;
    unsigned int size;
    struct memory_block* next;
};