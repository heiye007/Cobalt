#ifndef KERNEL_MEMORY_H_
#define KERNEL_MEMORY_H_ 

#define NULL 0
#define MEM_BLOCK_SIZE sizeof(struct memory_block)

/**
 * @brief Clear x bytes starting at z position of memory
 * @param addr base addres to start clearing
 * @param size number of bytes to clear
 */
#define kzero_memory(addr, size) (kmemset((addr), 0, (size)))

struct memory_block {
    char free;
    unsigned int size;
    struct memory_block* next;
};


/**
 * @brief Allocate a memory buffer in memory
 *
 * Calling this function will return a pointer to a memory buffer that can be used for
 * any purpose. The function will allocate the requested size. Because there is no
 * memory protection system at this level, it is not guaranteed the results of accessing
 * locations outside the boundaries of the requested buffer.
 *
 * @param size how many bytes to allocate.
 * @return the pointer to the memory buffer.
 */
void* kmalloc(unsigned int size);

/**
 * @brief Free memory block (after free can be reused)
 * @param addr Adress of the pointer to the block + MEM_BLOCK_SIZE
 */
void kfree(void* addr);

/**
 * @brief Find free memory blocks
 * @param size requires size for the block
 * @return pointer to block if exists else 0 (NULL)
 */
void* kfind_free_block(unsigned int size);

/**
 * @brief Set byte to a memory region
 * @param position, pointer to memory region start
 * @param byte, new value to set
 * @param nbytes, number of bytes to set starting from position
 */
void kmemset(void* poistion, char byte, unsigned int nbytes);

#endif