#include <stdint.h>
#include <types.h>
#include <keyboard.h>

struct SKeys {
  uint16_t shift : 1;
} keys;

int kbd_state = 0;
char curr_char = NULL;

void keyboard_handler(struct regs *r) {
    uint16_t scancode;
    uint16_t scancodebuf[5];
    uint16_t *sc = scancodebuf;

    *sc = inb(0x60);

    if (*sc & 0x80) {
      switch(*sc) {
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
      switch(*sc) {
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

char getch(void) {
  curr_char = NULL;
  while(!curr_char) printf("");
  return curr_char;
}

void init_keyboard(void) {
    irq_install_handler(1, keyboard_handler);
}