#include <i386/paging.h>
#include <i386/kframe.h>
#include <i386/kmalloc_early.h>
#include <i386/panic.h>
#include <stdint.h>
#include <stddef.h>

// Array to track free frames on stack.
uint32_t stack_count = 0; // Current stack capacity.
// Free frames pointer
uint32_t *free_frames = NULL;
int32_t top_of_stack = -1;
// If stack is empty, allocate from the end of the physical memory.
// Since we've not allocated any pages, end_of_mem begins at 0.
uint32_t end_of_mem = 0;
uint32_t allocated_frames = 0;
uint32_t total_frames;

void init_frame_allocator(uint32_t system_frames)
{
    total_frames = system_frames;

    if(free_frames != NULL)
    {
        // Frame allocator already initialized.
        return;
    }

    // Allocate a big stack to hold all the frames (Should be at most 4 MiB).
    free_frames = (uint32_t *)e_kmalloc(system_frames * sizeof (uint32_t));
    stack_count = system_frames;
}

void alloc_frame(page *page, int is_kernel, int is_writeable)
{
    if (page->frame != 0)
    {
        return; // Frame already allocated.
    }

    allocated_frames++;
    uint32_t idx;
    if(top_of_stack >= 0)
    {
        // Free frames exist on stack.
        idx = free_frames[top_of_stack];
        top_of_stack--; // pop [esp].
    }
    else
    {
        // No free frames on the stack.
        // Allocate one from the end of memory region.
        if(end_of_mem >= total_frames)
        {
            // There are no free frames in the frame stack
            // and we're at the limit of our physical memory.
            /* TODO: Implement a smart memory manager to avoid
               memory allocation issues. */
            PANIC("Out of Memory: Cannot allocate more frames");
        }

        idx = end_of_mem;
        end_of_mem++;
    }
    page->present = 1;						// Mark page as present (Active).
    page->rw      = (is_writeable) ? 1 : 0; // Writeable?
    page->user    = (is_kernel) ? 0 : 1;    // Kernel/User?
    page->frame   = idx;
}

void free_frame(page *page)
{
	// Put the freed frame into the stack.
    top_of_stack++; // push [esp].

    if(((uint16_t)top_of_stack) >= stack_count)
    {
        // This should never happen because we're allocating
        // the full stack during initialization.
        PANIC("Full frame pool");
    }

    free_frames[top_of_stack] = page->frame;
    page->frame = 0x0;
    allocated_frames--;
}