#include <i386/vga.h>
#include <string.h>

void printf(char* fmt, ...)
{
    if (fmt)
    {
        va_list list;
        va_start(list, fmt);

        char* ch;

        for (ch = fmt; *ch; ch++)
        {
            if (*ch != '%')
            {
                putch(*ch);
            }
            else
            {
                int d_num;
                uint16_t u_num;
                char* d_str;
                ch++;

                switch (*ch)
                {
                    case 'c':
                        d_num = va_arg(list, int);
                        putch(d_num);
                        break;
                    case 'd':
                        d_num = va_arg(list, int);
                        printkint(d_num);
                        break;
                    case 's':
                        d_str = va_arg(list, char*);
                        printk(d_str);
                        break;
                    case 'u':
                        u_num = va_arg(list, uint16_t);
                        printkuint(u_num);
                        break;
                    case 'x':
                        u_num = va_arg(list, uint16_t);
                        printkhex(u_num);
                        break;
                    default:
                        putch('%');
                        putch(*ch);
                        break;
                }
            }
        }
    }
}