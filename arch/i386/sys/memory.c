#include <i386/memory.h>

extern char kernel_end;

static void* next_address = &kernel_end;
struct memory_block* last;

void* kfind_free_block(unsigned int size)
{
    struct memory_block* current = &kernel_end;
    struct memory_block* best_block = NULL;
    char found = 0;

    while (current)
    {
        if (current->free && size <= current->size && (!best_block || current->size < best_block->size))
        {
            best_block = current;
        }
        current = current->next;
    }

    return best_block;
}

void kfree(void* addr)
{
    struct memory_block* block = addr - MEM_BLOCK_SIZE;
    block->free = 1;
}

void* kmalloc(unsigned int size)
{
    if (size == 0)
        return NULL;

    struct memory_block* block = kfind_free_block(size); 


    if (!block)
    {
        block = (struct memory_block*)next_address; 

        block->size = size;

        next_address += MEM_BLOCK_SIZE + size;
    }
    
    if (last)
    {
        last->next = block; 
    }
    
    last = block;
    block->free = 0;

    return (void*)block + MEM_BLOCK_SIZE;
}


void kmemset(void* position, char byte, unsigned int nbytes)
{
    void* end = position + nbytes;
    while (position < end)
    {
        *(char*)(position) = byte; 
        position++;
    }
}