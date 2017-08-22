#include <stdint.h>
#include <types.h>
#include <keyboard.h>

int kbd_state = 0;
char curr_char = NULL;

void keyboard_handler(struct regs *r) {
    uint8_t scancode;

    scancode = inb(0x60);

    if (scancode & 0x80) {
        // Key Release Control
    } else {
        curr_char = (keycode[scancode]);
    }
}

char getch() {
  curr_char = NULL;
  while(!curr_char) printf("");
  return curr_char;
}

void init_keyboard(void) {
    irq_install_handler(1, keyboard_handler);
}