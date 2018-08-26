#include <i386/a20.h>
#include <i386/vga.h>

int a20_enable_fast()
{
    uint8_t temp = inb(0x92);
    temp |=  0x02;
    temp &= ~0x01;
    outb(temp, 0x92);
    printkok("Initialized A20 Line using Fast Gate");
    return 1;
}

int a20_enable_ps2()
{
    uint8_t temp;
    
    while ((inb(0x64) & 0x2 ) != 0 );
    outb(0x64, 0xAD);

    while ((inb(0x64) & 0x2 ) != 0 );
    outb(0x64, 0xD0);

    while ((inb(0x64) & 0x1) == 0 );
    temp = inb( 0x60 );
    temp |= 0x2;

    while ((inb(0x64) & 0x2 ) != 0 );
    outb(0x64, 0xD1);

    while ((inb(0x64) & 0x2 ) != 0 );
    outb(0x60, temp);

    while ((inb(0x64) & 0x2 ) != 0 );
    outb(0x64, 0xAE);

    printkok("Initialized A20 Line using PS2 Controller Command");
    return 1;
}

void init_a20()
{
    uint32_t a20;
    a20 = check_a20();

    if (a20)
    {
        a20_enable_bios();
        if (check_a20())
        {
            printkok("Initialized A20 Line using BIOS");
            return;
        }
        else
        {
            a20_enable_ps2();
            if (check_a20())
            {
                printkok("Initialized A20 Line using PS2 Controller");
                return;
            }
            else
            {
                a20_enable_fast();
                if (check_a20())
                {
                    printkok("Initialized A20 Line using Fast Gate");
                    return;
                }
                else
                {
                    printkfail("Couldn't enable A20 Line!\n");
                }
            }
        }
    }
}