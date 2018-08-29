#include <i386/gfx.h>
#include <i386/vga.h>
#include <string.h>

// We need a method to automatically gather those 3 variables
int currmode = 0;
int currheight = 0;
int currwidth = 0;

void x86_screen_write_regs(uint8_t* regs)
{
    unsigned i;
    outb(VGA_MISC_WRITE, *regs);
    regs++;

    for (i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
        outb(VGA_SEQ_INDEX, i);
        outb(VGA_SEQ_DATA, *regs);
        regs++;
    }
    
    outb(VGA_CRTC_INDEX, 0x03);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
    outb(VGA_CRTC_INDEX, 0x11);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
    regs[0x03] |= 0x80;
    regs[0x11] &= ~0x80;
    
    for (i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
        outb(VGA_CRTC_INDEX, i);
        outb(VGA_CRTC_DATA, *regs);
        regs++;
    }
    
    for (i = 0; i < VGA_NUM_GC_REGS; i++)
    {
        outb(VGA_GC_INDEX, i);
        outb(VGA_GC_DATA, *regs);
        regs++;
    }
    
    for (i = 0; i < VGA_NUM_AC_REGS; i++)
    {
        (void)inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, *regs);
        regs++;
    }
    
    (void)inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, 0x20);
}

void x86_update_screen_mode_info(int mode)
{
    switch(mode)
    {
        case(TEXT_80x25):
            currmode = TEXT_80x25; // 80x25 Text Mode
            currheight = 80;
            currwidth = 25;
            break;
        case(SD_320x200x256):
            currmode = SD_320x200x256; // 320x200x256 SD Mode
            currheight = 320;
            currwidth = 200;
        default:
            break;
    }
}

void x86_switch_screen_mode_command(char *regs)
{
    if (!strcmp(regs, "modes")) {
        /* Make mode gathering automatic
           either by using multiboot header
           or using GPU and Monitor values
           or maybe EDID? */
        printkc("Available VESA modes:\n", LBLUE);
        printkc("Height     Width     BPP\n", YELLOW);
        printk("320     x  200    x  256\n");
        printk("80      x  25\n");
    } else if (!strcmp(regs, "320x200x256")) {
        if (currmode == SD_320x200x256) {
            printkc("You can't set a mode you're already using!\n", RED);
        } else {
            x86_screen_write_regs(g_320x200x256);
            vbe_clear_screen(9);
            vbe_draw_rectangle(0, 0, 320, 10, 8); // Title Bar
            vbe_printk("cobalt vbe test");
            x86_update_screen_mode_info(SD_320x200x256);
        }
    } else if (!strcmp(regs, "80x25")) {
        if (currmode == TEXT_80x25) {
            printkc("You can't set a mode you're already using!\n", RED);
        } else {
            x86_screen_write_regs(g_80x25_text);
            setup_text_mode();
        }
    } else if (!strcmp(regs, "") || !strcmp(regs, "help")) {
        printkc("Usage:\n", LBLUE);
        printk("vbe MODE\n");
        printkc("For available modes, use:\n", LBLUE);
        printk("vbe modes\n");
        printkc("Example:\n", LBLUE);
        printk("vbe 320x200x256\n");
    } else {
        printkc("VESA mode not found!\n", RED);
    }
}