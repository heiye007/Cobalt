#include <i386/gfx.h>
#include <multiboot.h>

#define VGA_AC_INDEX        0x3C0
#define VGA_AC_WRITE        0x3C0
#define VGA_AC_READ     0x3C1
#define VGA_MISC_WRITE      0x3C2
#define VGA_SEQ_INDEX       0x3C4
#define VGA_SEQ_DATA        0x3C5
#define VGA_DAC_READ_INDEX  0x3C7
#define VGA_DAC_WRITE_INDEX 0x3C8
#define VGA_DAC_DATA        0x3C9
#define VGA_MISC_READ       0x3CC
#define VGA_GC_INDEX        0x3CE
#define VGA_GC_DATA     0x3CF

#define VGA_CRTC_INDEX      0x3D4
#define VGA_CRTC_DATA       0x3D5
#define VGA_INSTAT_READ     0x3DA

#define VGA_NUM_SEQ_REGS    5
#define VGA_NUM_CRTC_REGS   25
#define VGA_NUM_GC_REGS     9
#define VGA_NUM_AC_REGS     21

extern unsigned char g_320x200x256[];
extern unsigned char g_720x480x16[];
int currline = 0;
int x = 0;

void vbe_write_regs(uint8_t* regs)
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

void set_vbe_palette_color(uint8_t color, uint8_t r, uint8_t g, uint8_t b)
{
    outb(0x03C6, 0xff);
    outb(0x03C8, color);
    outb(0x03C9, r);
    outb(0x03C9, g);
    outb(0x03C9, b);
}

unsigned get_screen_fb()
{
    unsigned seg;
    outb(VGA_GC_INDEX,6);
    seg = inb(VGA_GC_DATA);
    seg >>= 2;
    seg &= 3;

    switch(seg)
    {
    case 0:
    case 1:
        seg = 0xA000;
        break;
    case 2:
        seg = 0xB000;
        break;
    case 3:
        seg = 0xB800;
        break;
    }
    return seg;
}

void vbe_write_pixel(unsigned x, unsigned y, unsigned c)
{
    unsigned wd_in_bytes;
    unsigned off;
    wd_in_bytes = 320;
    off = wd_in_bytes * y + x;
    char* vgamemory = (char*)0xA0000;
    vgamemory[off] = c;
}

void vbe_clear_screen(char c)
{
    int x = 0, y = 0;

    for (y = 0; y < 200; y++)
    {
        for (x = 0; x < 320; x++)
        {
            vbe_write_pixel(x, y, c);
        }
    }
}

void vbe_draw_rectangle(int startx, int starty, int endx, int endy, char c)
{
    int x = 0,y=0;

    for(y = starty; y <= endy; y++)
    {
        for(x = startx; x <= endx; x++)
        {
            vbe_write_pixel(x, y, c);
        }
    }
}

void vbe_drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor)
{
    int cx, cy;
    int mask[8] = {1, 2, 4, 8, 16, 32, 64, 128};
    unsigned char *glyph = ((unsigned char*)g_320x200x256_font) + (int32_t)(c - 32) * 8;

    for(cy = 0; cy < 8; cy++)
    {
        for(cx = 0; cx < 8; cx++)
        {
            vbe_write_pixel(x - cx, y + cy - 6, glyph[cy]&mask[cx] ? fgcolor : bgcolor);
        }
    }
}

void vbe_putchar(char c, int row, int col)
{
    vbe_drawchar(c, col * 8 + 8, row * 16 + 12, 0, 9);
}

uint32_t vbe_printk(char *message)
{
    while(*message != 0)
    {
        if(*message == '\n')
        {
            currline++;
            x++;
            message++;
            continue;
        }

        if(x >= 320/8-1)
        {
            currline++;
            x = 0;
        }

        vbe_putchar(*message, currline, x);
        message++;
        x++;
    }

    return currline;
}