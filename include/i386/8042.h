#include <stdint.h>

char init_8042_keyboard();
#define UNUSED(x) (void)(x)


enum SPECIAL
{
  LCTRL = 0x1D, RCTRL = 0xE0,
  LSHIFT = 0x2A, RSHIFT = 0x36,
  LALT = 0x11, RALT = 0x38,
  LSUPER = 0x5C, RSUPER = 0x5B,
  BACKSPACE = 0x0E, ENTER = 0x1C,
  ARW_UP = 0x48, ARW_LEFT = 0x4B, ARW_RIGHT = 0x4D, ARW_DOWN = 0x50
};
