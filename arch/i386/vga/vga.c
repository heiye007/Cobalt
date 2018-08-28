#include <i386/vga.h>
#include <i386/system.h>
#include <i386/panic.h>
#include <i386/gfx.h>
#include <string.h>

#define WIDTH 80
#define ROWS 25

unsigned short *textmemptr = (unsigned short *)0xB8000;
int attrib = 0x0F;
int row = 0;
int col = 0;

void scroll(void)
{
    uint8_t attributeByte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attributeByte << 8);

    if(col >= ROWS)
    {
        int i;

        for (i = 0*WIDTH; i < (ROWS-1)*WIDTH; i++)
        {
            textmemptr[i] = textmemptr[i+WIDTH];
        }

        for (i = (ROWS-1)*WIDTH; i < ROWS*WIDTH; i++)
        {
            textmemptr[i] = blank;
        }

        col = (ROWS-1);
    }
}

void move_csr(void)
{
    unsigned temp;
    temp = col * WIDTH + row;
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}

void update_cursor(int newrow, int newcol)
{
    unsigned short position = (newrow*80) + newcol;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position&0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position>>8)&0xFF));
    row = newrow;
    col = newcol;
    move_csr();
}

int get_row()
{
    return row;
}

int get_col()
{
    return col;
}

void disable_cursor(void)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x3f);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3E0) & 0xE0) | cursor_end);
}

void cls(void)
{
    update_cursor(0, 0);
	int i = 0;
	for (i = 0; i < WIDTH * ROWS; i++)
		textmemptr[i] = (attrib << 8) | 0x20;
}

void printkcenter(char* c)
{
    int size = 0;

    while(c[size])
    {
        size++;
    }

    int half;

    if(size % 2 != 0)
    {
        half = size / 2 -1;

        for(int i = 0; i < 40 - half - 1; i++)
        {
            putch(' ');
        }

        printk(c);

        for(int i = 0; i < 40 - half - 2; i++)
        {
            putch(' ');
        }
    }
    else
    {
        half = size / 2;

        for(int i = 0; i < 40 - half; i++) 
        {
            putch(' ');
        }
        
        printk(c);
        
        for(int i = 0; i < 40 - half; i++)
        {
            putch(' ');
        }
    }
}

void printkc(char *text, int colour)
{
    settextcolor(colour, BLACK);
    printk("%s", text);
    settextcolor(WHITE, BLACK);
}

void printkok(char *text)
{
    printk("%s", text);
    update_cursor(WIDTH-6, get_col());
    printk("[ ");
    settextcolor(GREEN, BLACK);
    printk("OK");
    settextcolor(WHITE, BLACK);
    printk(" ]");
}

void printkfail(char *text)
{
    printk("%s", text);
    update_cursor(WIDTH-6, get_col());
    printk("[");
    settextcolor(RED, BLACK);
    printk("FAIL");
    settextcolor(WHITE, BLACK);
    printk("]");
}

void putch(char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    if(c == 0x08)
    {
        if(row != 0) row--;
    }
    else if(c == 0x09)
    {
        row = (row + 8) & ~(8 - 1);
    }
    else if(c == '\r')
    {
        row = 0;
    }
    else if(c == '\n')
    {
        row = 0;
        col++;
    }
    else if(c >= ' ')
    {
        where = textmemptr + (col * WIDTH + row);
        *where = c | att;
        row++;
    }

    if(row >= WIDTH)
    {
        row = 0;
        col++;
    }

    scroll();
    move_csr();
}

void print(char *text)
{
    unsigned int i;

    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

void settextcolor(uint8_t forecolor, uint8_t backcolor)
{
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void setup_text_mode(void)
{
    disable_cursor();
    cls();
    update_cursor(0, 0);
    enable_cursor(14, 15);
    update_mode_info(TEXT_80x25);
}

void printkint(const int number)
{
    char buf[20];
    buf[0] = 0;
    int len = 0;
    int abs_value = number;

    if (number < 0)
    {
        abs_value = -number;
    }

    int last_digit;

    while (abs_value > 9)
    {
        last_digit = abs_value % 10;
        abs_value /= 10;
        buf[++len] = (char) last_digit + '0';
    }

    buf[++len] = (char) abs_value + '0';

    if (number < 0)
    {
        putch('-');
    }

    while (len > 0)
    {
        putch(buf[len--]);
    }
}

void printkdec(uint16_t n)
{
    if (n == 0)
    {
        putch('0');
        return;
    }

    int acc = n;
    char c[32];
    int i = 0;
    
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }

    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;

    while(i >= 0)
    {
        c2[i--] = c[j++];
    }

    print(c2);
}

char hex_letters[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

void printkhex(const uint16_t number)
{
    char buf[20];
    buf[0] = 0;
    int len = 0;

    uint16_t value = number;

    while (value >= 16)
    {
        buf[++len] = hex_letters[value & 0xF];
        value >>= 4;
    }

    buf[++len] = hex_letters[value & 0xF];

    while (len)
    {
        putch(buf[len--]);
    }
}

void printkuint(const uint16_t number)
{
    char buf[20];
    buf[0] = 0;
    int len = 0;

    uint16_t value = number;

    while (value >= 10)
    {
        int last_digit = value % 10;
        value /= 10;
        buf[++len] = '0' + last_digit;
    }

    buf[++len] = '0' + value;

    while (len)
    {
        putch(buf[len--]);
    }
}