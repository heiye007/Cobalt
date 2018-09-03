#include <multiboot.h>
#include <stdint.h>

const char* mem_type_names[] = {"", "Available", "Reserved", "ACPI", "NVS", "Bad RAM"};

void get_memory_map(multiboot_info_t *info)
{
	multiboot_memory_map_t* mmap = info->mmap_addr;
	while(mmap < info->mmap_addr + info->mmap_length)
    {
		uint32_t addr = (uint32_t)mmap->addr;
		uint32_t len = (uint32_t)mmap->len;

		printk("0x%x - 0x%x | %s\n", addr, addr + len, mem_type_names[mmap->type]);

		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
	}
}