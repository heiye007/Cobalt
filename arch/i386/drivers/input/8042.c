#include <i386/8042.h>
#include <i386/vga.h>
#include <i386/system.h>
#include <i386/regs.h>
#include <stdint.h>
#include <types.h>

extern void x86_register_interrupt_handler(int irq, void (*handler));

static uint8_t uppercase = 0;
static uint8_t lastwasmagic = 0;

#define SC_MAX 57

const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
  "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
    "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
    "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
    "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
    "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',
  '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
    'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', '?', '*', '?', ' '};

const char sc_shift_below[] = { '?', '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '?', '?'};

const char sc_shift_middle[] = { ':', '"', '~', '?', '|'}; //0x27 to 0x2B

const char sc_shift_above[] = { '<', '>', '?', '?', '*', '?', ' '}; //0x33 to 0x39

static void keyboard_callback(registers_t regs) {
  /* The PIC leaves us the scancode in port 0x60 */
  uint8_t scancode = inb(0x60);
  uint8_t special = 1;
  if (scancode == 0xe0)
  {
    lastwasmagic = 1;
  }
  else if (lastwasmagic)
  {
    keyboard_handler_special(scancode);
    lastwasmagic = 0;
  }
  else
  {
    if (scancode > SC_MAX)
    { //Key being released
      //0x80 = difference between release code and press code
      if (scancode - 0x80 == LSHIFT || scancode - 0x80  == RSHIFT)
      {
        uppercase = 0;
      }
    }
    else {
      if (scancode == LSHIFT || scancode == RSHIFT)
      {
        uppercase = 1;
      }
      else
      {
        char letter = sc_ascii[(int)scancode];
        if (letter >= 65 && letter <= 90) {
          letter += 32 * !uppercase; //Remove 32 from letter(make it lower case) if uppercase is false
          special = 0;
        }
        else if (uppercase)
        {
          if (scancode < 0x10)
          {
            special = 0;
            letter = sc_shift_below[scancode];
          }
          else if (scancode >= 0x27 && scancode <= 0x2B)
          {
            special = 0;
            letter = sc_shift_middle[scancode - 0x27];
          }
          else if (scancode >= 0x33 && scancode <= 0x39)
          {
            special = 0;
            letter = sc_shift_above[scancode - 0x33];
          }
        }
        else if ((scancode >= 0x02 && scancode <= 0x0D) || (scancode >= 0x27 && scancode <= 0x29) ||
             (scancode >= 0x33 && scancode <= 0x35) || scancode == 0x2B || scancode == 0x39) {
          special = 0;
        }
        
        if(special)
        {
          keyboard_handler_special(scancode);
        }
        else
        {
          keyboard_handler(letter);
        }
      }
    }
  }
  UNUSED(regs);
}

void ps2_flush_buffer()
{
	inb(0x60); 
	inb(0x60); 
	inb(0x60); 
	inb(0x60);
}

char init_8042_keyboard()
{
    x86_register_interrupt_handler(1, keyboard_callback); /* IRQ1: Keyboard */
	
	while (inb(0x64) & 1) // Clear Keyboard Buffer
	{
		inb(0x60);
	}

    printkok("Initialized PS2 (8042) Keyboard");
    
	return 0;
}