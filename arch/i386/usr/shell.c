#include <i386/vga.h>
#include <i386/cpu.h>
#include <i386/kheap.h>
#include <i386/gfx.h>
#include <i386/8042.h>
#include <multiboot.h>
#include <string.h>
#include <stdint.h>

int lastlinedetect;

uint32_t shell()
{
  char c, line[80], cmd[80], arg[80];
  printk("Running Shell...\n");

  while(1)
  {
    printk("$ ");
    lastlinedetect = 0;
    memset(line, '\0', 80);
    memset(cmd, '\0', 80);
    memset(arg, '\0', 80);

    while((c = getch()) != '\n')
    {
      // cake: Well, backspacing 'works' using
      // nasty hacks, we can't rely on integer
      // comparison for checking if we 'arrived'
      // to the shell sign. We should use a buffer
      // so it stays as a known-bug without a proper fix.
      if (c == '\b')
      {
        if (lastlinedetect == 0) {
          continue;
        } else {
          if (get_row() == 0) {
            lastlinedetect--;
            update_cursor(79, get_col()-1);
            putch(' ');
            update_cursor(80, get_col()-1);
          } else {
            lastlinedetect--;
            update_cursor(get_row()-1, get_col());
            putch(' ');
        }
        }
      }
      else
      {
        line[lastlinedetect] = c;
        lastlinedetect++;
      }
      putch(c);
    }

    putch(c);
    line[lastlinedetect] = '\0';

    int pos = 0;

    while(line[pos] != ' ' && line[pos] != '\0') pos++;

    memcpy(cmd, line, pos);
    memcpy(arg, line + pos + 1, lastlinedetect - pos);
    cmd[pos] = '\0';
    arg[lastlinedetect - pos] = '\0';

    if (!strcmp(cmd, "hello"))
    {
      printk("Hello!\n");
    }
    else if (!strcmp(cmd, "panic"))
    {
      int a = 10;
      int b = 0;
      int c = a/b;
      printk("%d", c);
    }
    else if (!strcmp(cmd, "vbe"))
    {
      extern unsigned char g_320x200x256[];
      init_vbe(g_320x200x256);
      vbe_clear_screen(9);
      vbe_draw_rectangle(0,0,320,10,8); // Title Bar
      vbe_printk("cobalt vbe test", 0);
    }
    else if (!strcmp(cmd, "pagefault"))
    {
      unsigned int *ptr = (unsigned int*)0xA0000000;
      unsigned int do_page_fault = *ptr;
      *ptr = NULL;
      do_page_fault = NULL;
      kmalloc(*ptr + do_page_fault);
    }
    else if (!strcmp(cmd, "cpuinfo"))
    {
      getCPUVendor();
      getCPUFeatures();
      getCPUArch();
      getCPUName();
    }
    else if (!strcmp(cmd, "debug"))
    {
      extern uint32_t x86_memoryhead, x86_memoryend, x86_total_bytes, x86_free_bytes, x86_byte_allocations, x86_initial_esp, x86_kernel_end, x86_kernel_start, x86_initrd_size, x86_initrd_start, x86_initrd_end;
      extern uint32_t x86_ramsize, x86_kernel_size, x86_kernel_iss, x86_kernel_ise, x86_kernel_isi, x86_ramstart, x86_memory_end_location, x86_memory_amount, x86_memory_location, x86_usable_mem;
      printk("Current Framebuffer: 0x%x\n", get_screen_fb());
      printk("Initial Stack Pointer: 0x%x\n", x86_initial_esp);
      printk("Kernel Base: 0x%x , Kernel End: 0x%x , Kernel Size: (%dKiB)\n", &x86_kernel_start, &x86_kernel_end, ((uint32_t) &x86_kernel_size) >> 10);
      printk("Boot Stack Base: 0x%x , Boot Stack End: 0x%x , Boot Stack Size: (%dKiB)\n", &x86_kernel_isi, &x86_kernel_ise, ((uint32_t) &x86_kernel_iss) >> 10);
      printk("Initrd start: 0x%x , Initrd end: 0x%x , Initrd Size: (", x86_initrd_start, x86_initrd_end);

      if(x86_initrd_size / 1024 / 1024 > 0)
      {
        printk("%d MiB)\n", x86_initrd_size / 1024 / 1024);
      }
      else if(x86_initrd_size / 1024 > 0)
      {
        printk("%d KiB)\n", x86_initrd_size / 1024);
      } else {
        printk("%d B)\n", x86_initrd_size);
      }

      printk("Heap Base: 0x%x , Heap End: 0x%x , Heap Size: (", x86_memoryhead, x86_memoryend);
      
      if(x86_total_bytes / 1024 / 1024 > 0)
      {
        printk("%d MiB)\nCurrent Heap Allocations: (%d)\n", x86_total_bytes / 1024 / 1024, x86_byte_allocations);
      }
      else if(x86_total_bytes / 1024 > 0)
      {
        printk("%d KiB)\nCurrent Heap Allocations: (%d)\n", x86_total_bytes / 1024, x86_byte_allocations);
      } else {
        printk("%d B)\nCurrent Heap Allocations: (%d)\n", x86_total_bytes, x86_byte_allocations);
      }

      if(x86_free_bytes / 1024 / 1024 > 0)
      {
        printk("Current Heap Free Memory: (%d MiB)\n", x86_free_bytes / 1024 / 1024);
      }
      else if(x86_free_bytes / 1024 > 0)
      {
        printk("Current Heap Free Memory: (%d KiB)\n", x86_free_bytes / 1024);
      } else {
        printk("Current Heap Free Memory: (%d B)\n", x86_free_bytes);
      }

        printk("Total System RAM: %d MB \n",  x86_ramsize);
        printk("Start of Physical RAM Memory    : 0x%x\n", (uint32_t) x86_ramstart);
        printk("End of Physical RAM Memory      : 0x%x\n", (uint32_t) x86_memory_end_location);
        printk("Psysical RAM Memory Amount Size : 0x%x\n", x86_memory_amount);
        printk("Usable RAM Memory Starts At     : 0x%x\n", (uint32_t) x86_memory_location);
        printk("Usable Amount of RAM Memory     : 0x%x\n", x86_usable_mem);
      
      }
      else
      {
        printk("Unknown command: %s\n", cmd);
      }
    }

  return 0;
}