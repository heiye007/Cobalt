#include <i386/isr.h>
#include <fs/fs.h>
#include <multiboot.h>
#include <panic.h>
#include <stdbool.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

extern uint32_t kernel_end;
extern uint32_t kernel_start;

void init(unsigned long magic, multiboot_info_t *mbi)
{
	init_vga();

#ifdef DBG_CPU
	getCPUVendor();
	getCPUFeatures();
	getCPUArch();
	getCPUName();
#endif
	uint32_t initrd_location = *((uint32_t*)mbi->mods_addr);
	uint32_t initrd_end = *(uint32_t*)(mbi->mods_addr+4);
	
	uint32_t low_pages = 256;
    uint32_t high_pages = (mbi->mem_upper * 1024) / 4096 + 30000;

    uint32_t total_frames = high_pages + low_pages;

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		printf("Invalid magic number: 0x%x\n", (unsigned) magic);
		__asm__ __volatile__ ("cli");
		__asm__ __volatile__ ("hlt");
		return;
	}

#ifdef DBG_INIT
	printf("Kernel base is 0x%x, end is 0x%x\n", &kernel_start, &kernel_end);
#endif

    multiboot_memory_map_t* mmap = mbi->mmap_addr;

    while(mmap < mbi->mmap_addr + mbi->mmap_length)
    {
        mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
    }

#ifdef DBG_INIT
    printf("RAM size: %d MB \n",  mbi->mem_upper / 1024 + 2);
    printf("PAGING size: %d \n",  high_pages);
#endif

	init_gdt();
	printkok("Initialized GDT");
	init_idt();
	printkok("Initialized IDT");
	init_isr();
	printkok("Initialized ISR's");
	initialize_paging(total_frames, 0, 0);
	printkok("Initialized Paging");
	malloc_stats();
	printkok("Initialized Memory Management Engine");
	init_irq();
	printkok("Initialized IRQ's");
	__asm__ __volatile__ ("sti");
	printkok("Interrupts enabled");
	pic_init();
	printkok("Initialized PIC");
	pit_init();
	printkok("Initialized PIT");
	init_keyboard();
	printkok("Initialized Keyboard");

	printf("Mods count: %d\n", mbi->mods_count);

	bool modules_exist = false;

	if (mbi->mods_count > 0)
	{
		modules_exist = true;
		fs_root = initialise_initrd(initrd_location);
	}
	else
	{
		printf("No Modules Found!");
	}
	
	if (modules_exist == 1)
	{
		// list the contents of /
		int i = 0;
		struct dirent *node = 0;
		while ( (node = readdir_fs(fs_root, i)) != 0)
		{
			printf("Found file ");
			printf(node->name);
			fs_node_t *fsnode = finddir_fs(fs_root, node->name);

			if ((fsnode->flags&0x7) == FS_DIRECTORY)
			{
				printf("\n\t(directory)\n");
			}
			else
			{
				printf("\n\t contents: \"");
				char buf[256];
				uint32_t sz = read_fs(fsnode, 0, 256, buf);
				int j;
		
				for (j = 0; j < sz; j++)
				{
					putch(buf[j]);
				}

				printf("\"\n");
			}

			i++;
		}
	}

	/* XXX: Legacy Paging Tester
	unsigned int *ptr = (unsigned int*)0xA0000000;
   	unsigned int do_page_fault = *ptr;*/

	shell();

	/* XXX: Kernel can't reach this zone,
	if it does, throw a Kernel Panic Error */
	PANIC("Reached Kernel End!");
	while(1);
}