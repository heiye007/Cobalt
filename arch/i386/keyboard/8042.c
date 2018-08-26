#include <i386/8042.h>
#include <i386/vga.h>
#include <i386/system.h>
#include <i386/regs.h>
#include <stdint.h>
#include <types.h>

extern void irq_install_handler(int irq, void (*handler));

uint8_t keycode[128] =
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b',
    '\t',
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0,
    '*',
    0,
    ' ',
    0,
    0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0,
    0,
    0,
    '-',
    0,
    0,
    0,
    '+',
    0,
    0,
    0,
    0,
    0,
    0, 0, 0,
    0,
    0,
    0,
};

struct SKeys
{
  uint16_t shift : 1;
} keys;

int kbd_state = 0;
char curr_char = NULL;

static void keyboard_handler(struct regs* r)
{
    uint16_t scancodebuf[5];
    uint16_t *sc = scancodebuf;

    *sc = inb(0x60);

    if (*sc & 0x80)
    {
      switch(*sc)
      {
        case 0xaa:
          keys.shift = 0;
          break;
        case 0xb6:
          keys.shift = 0;
          break;
        default:
          break;
        }
      } else {
      switch(*sc)
      {
        case 0x2A:
          keys.shift = 1;
          break;
        case 0x36:
          keys.shift = 1;
          break;
        default:
          if (keys.shift)
            curr_char = (keycode[*sc]-0x20);
          else
            curr_char = (keycode[*sc]);
    }
  }
}

char getch(void)
{
  curr_char = NULL;
  while(!curr_char) printk("");
  return curr_char;
}

void init_8042_keyboard(void)
{
    irq_install_handler(1, keyboard_handler);
    printkok("Initialized PS2 (8042) Keyboard");
}