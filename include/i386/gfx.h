#include <i386/system.h>
#include <i386/font.h>
#include <stdint.h>

// gfx.c
void init_text_mode();
void init_vbe(uint8_t* regs);

// vbe.c
void vbe_write_regs(uint8_t* regs);
void set_vbe_palette_color(uint8_t color, uint8_t r, uint8_t g, uint8_t b);
unsigned get_screen_fb();
void vbe_write_pixel(unsigned x, unsigned y, unsigned c);
void vbe_clear_screen(char c);
void vbe_draw_rectangle(int startx, int starty, int endx, int endy, char c);
void vbe_drawchar(unsigned char c, int x, int y, int fgcolor, int bgcolor);
void vbe_putchar(char c, int row, int col);
uint32_t vbe_printk(char *message);