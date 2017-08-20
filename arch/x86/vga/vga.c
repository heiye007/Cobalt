#include <stdarg.h>
#include <stdint.h>

/* This values are set for 80x25 QEMU mode */
#define WIDTH 80
#define ROWS 25

/* These define textpointer, background, foreground
   colors (attributes) and cursor coordinates (x,y) */

unsigned short *textmemptr = (unsigned short *)0xB8000;
int attrib = 0x0F;
int csr_x = 0;
int csr_y = 0;

void scroll(void)
{
    unsigned blank, temp;

    /* Background color for blank */
    blank = 0x20 | (attrib << 8);

    if(csr_y >= ROWS)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = csr_y - ROWS + 1;
        memcpy (textmemptr, textmemptr + temp * WIDTH, (ROWS - temp) * WIDTH * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memsetw (textmemptr + (ROWS - temp) * WIDTH, blank, WIDTH);
        csr_y = ROWS - 1;
    }
}

void move_csr(void)
{
    /* Updates the hardware cursor */
    unsigned temp;

    /* The equation for finding the index in a linear
       chunk of memory can be represented by:
       Index = [(y * width) + x] */
    temp = csr_y * WIDTH + csr_x;

    /* This sends a command to indicies 14 and 15 in the
    *  CRT Control Register of the VGA controller. These
    *  are the high and low bytes of the index that show
    *  where the hardware cursor is to be 'blinking' */
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}

void cls(void)
{
	int i = 0;
	for (i = 0; i < WIDTH * ROWS; i++)
		textmemptr[i] = (attrib << 8) | 0x20;
}

void putch(char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Handle a backspace, by moving the cursor back one space */
    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    }
    /* Handles a tab by incrementing the cursor's x, but only
    *  to a point that will make it divisible by 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* Handles a 'Carriage Return', which simply brings the
    *  cursor back to the margin */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* We handle our newlines the way DOS and the BIOS do: we
    *  treat it as if a 'CR' was also there, so we bring the
    *  cursor to the margin and we increment the 'y' value */
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Any character greater than and including a space, is a
    *  printable character. The equation for finding the index
    *  in a linear chunk of memory can be represented by:
    *  Index = [(y * width) + x] */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * WIDTH + csr_x);
        *where = c | att;	/* Character AND attributes: color */
        csr_x++;
    }

    /* If the cursor has reached the edge of the screen's width, we
    *  insert a new line in there */
    if(csr_x >= WIDTH)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Scroll the screen if needed, and finally move the cursor */
    scroll();
    move_csr();
}

void printk(char *text)
{
    int i;
    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void init_vga(void)
{
    /* May seem stupid to keep here only one func but it'll be better with GUI */
    cls();
}

void printkdec(unsigned int n)
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
    printk(c2);

}

void printkhex(unsigned int n)
{
    int tmp;

    printk("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            putch (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            putch( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        putch (tmp-0xA+'a');
    }
    else
    {
        putch (tmp+'0');
    }

}

void printf(char* fmt, ...)
{
    if (fmt) { /* Safety nullcheck */
        /* The hardest part here is to extract the variadic argument. */
        va_list list;
        va_start(list, fmt);

        /* Now it's just parsing fmt until we get a NUL character. */
        char* ch;
        for (ch = fmt; *ch; ch++) {
            if (*ch != '%') { /* Not a placeholder, safe to print? */
                putch(*ch);
            } else { /* Uh, oh, hold on. (Get it? Hold... nevermind) */
                int d_num;
                unsigned int u_num;
                char* d_str;
                ch++;
                switch (*ch) {
                    case 'c': // print a character.
                        d_num = va_arg(list, int);
                        putch(d_num);
                        break;
                    case 'd': // print a number
                        d_num = va_arg(list, int);
                        printkdec(d_num);
                        break;
                    case 's': // print a string.
                        d_str = va_arg(list, char*);
                        printk(d_str);
                        break;
                    case 'x':
                        u_num = va_arg(list, unsigned int);
                        printkhex(u_num);
                        break;
                    default: // unrecognized, what to do?
                        putch('%');
                        putch(*ch);
                        break;
                }
            }
        }
    }
}