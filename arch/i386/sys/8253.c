#include <i386/8253.h>
#include <i386/system.h>
#include <i386/vga.h>
#include <i386/regs.h>

extern void irq_install_handler(int irq, void (*handler));

/* timer_ticks keeps track of all the ticks that
   will happen since PIT initialization */
uint32_t timer_ticks = 0;

/* cake: We should code a suitable RTC driver
   for handling clock operations */
// Seconds Declaration
uint32_t seconds = 0;
// Minutes Declaration
uint32_t minutes = 0;
// Hours Declaration
uint32_t hours = 0;

// Timer Hertz's
uint32_t timer_hz = 0;

void timer_phase(int hz)
{
    timer_hz = hz;
    int divisor = 1193180 / hz;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}

/* Timer handler function with several routines */
void timer_handler()
{
    /* Increment our ticks variable */
    timer_ticks++;

    // XXX: Top Line Debugging for Cobalt
    // XXX: Example that shows column, row & lastlinedetect
    // cake: Used this to debug backspacing
#ifdef DBG_PIT_TOPLINE_VGA
    extern int lastlinedetect;

    int old_row = get_row();
    int old_col = get_col();
    update_cursor(0,0);
    settextcolor(BLACK, LGRAY);

    // cake: We don't have multitasking yet, very slow method to fill the top bar.
    for (int loop = 0; loop < 80; loop++)
    {
        putch(' ');
    }

    update_cursor(0,0);

    if (lastlinedetect == 0)
    {
       printk("Current row: %d , Current col: %d, lastlinedetect = %d, Detection Happened", old_row, old_col, lastlinedetect); 
    }
    else
    {
        printk("Current row: %d , Current col: %d, lastlinedetect = %d", old_row, old_col, lastlinedetect); 
    }

    settextcolor(WHITE, BLACK);
    update_cursor(old_row, old_col);
#endif

#ifdef DBG_PIT_TOPLINE_UPTIME
    // XXX: Example that shows up time in clock-based data
    int old_row = get_row();
    int old_col = get_col();
    update_cursor(0,0);
    settextcolor(BLACK, LGRAY);

    // cake: We don't have multitasking yet, very slow method to fill the top bar.
    for (int loop = 0; loop < 80; loop++)
    {
        putch(' ');
    }
    
    update_cursor(0,0);

    if (minutes == 0)
    {
        printk("Uptime: %d second(s)", seconds); 
    }
    
    if (hours == 0)
    {
        if (minutes != 0)
        {
            printk("Uptime: %d minute(s) and %d second(s)", minutes, seconds);
        }
    }

    if (hours != 0)
    {
        if (hours != 24)
        {
            printk("Uptime: %d hour(s) %d minute(s) and %d second(s)", hours, minutes, seconds);
        }
    }

    settextcolor(WHITE, BLACK);
    update_cursor(old_row, old_col);

    /* Every 18 ticks, update the uptime message */
    if (timer_ticks % timer_hz == 0)
    {
        seconds++;
        if (seconds == 60)
        {
            seconds = 0;
            minutes++;
        }

        if (minutes == 60)
        {
            minutes = 0;
            hours++;
        }
    }
#endif

}

uint32_t get_ticks()
{
	return timer_ticks;
}

/* Function to wait 'x' ticks */
void timer_wait(int ticks)
{
    int eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks)
    {
        // Put the CPU to sleep for saving power while waiting
        __asm__ __volatile__ ("sti//hlt//cli");
    }
}

void pit_init(void)
{
    irq_install_handler(0, timer_handler);
    timer_phase(100); // Set to 100Hz
    printkok("Initialized PIT");
}