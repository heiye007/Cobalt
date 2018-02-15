#include <stdarg.h>
#include <stdint.h>
#include <types.h>

enum vga_color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LGRAY = 7,
    DGRAY = 8,
    LBLUE = 9,
    LGREEN = 10,
    LCYAN = 11,
    LRED = 12,
    LMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

void scroll(void);
void move_csr(void);
void update_cursor(int newrow, int newcol);
int get_row();
int get_col();
void disable_cursor(void);
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void cls(void);
void printkcenter(char* c);
void printkok(char *text);
void printkfail(char *text);
void putch(char c);
void printk(char *text);
void settextcolor(uint8_t forecolor, uint8_t backcolor);
void init_vga(void);
void printkint(const int number);
void printkdec(uint16_t n);
void printkhex(const uint16_t number);
void printkuint(const uint16_t number);
void printf(char* fmt, ...);