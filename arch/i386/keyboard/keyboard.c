#include <stdint.h>

#define DEBUG 0
#define KB_DATA     0x60
#define KB_STAT     0x64
#define KB_CMD      0x64
#define TTY_EOF     254
#define TTY_INT     255
#define KB_STAT_OBF     0x01

#define SHIFT       (1<<0)
#define CTRL        (1<<1)
#define ALT         (1<<2)

#define E0ESC       (1<<6)

#define KB_IS_RELEASE(sc)   (sc & 0x80) 
#define KB_IS_ESCAPE(sc)    (sc == 0xe0)

static uint32_t kb_mode = 0;

static char kb_map[128] = {
    0,  
    0x1b,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',    
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',

    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`',
    0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/',   
    0,
    '*',
    0,
    ' ',
    0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
    0,   0,   0,
    0,
    0,
    0,
};

static char kb_shift_map[128] =
{
    0,  
    0x1b,
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R',    
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':','\"', '~',
    0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N',
    'M', '<', '>', '?',   
    0,
    '*',
    0,
    ' ',
    0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

static char shift(char sc) {
    char ch = sc & 0x7f;

    if (kb_mode & E0ESC) { 
        switch (ch) {
            case 0x1D: return CTRL;
            case 0x38: return ALT;
        }
    } else {
        switch(ch) {
            case 0x2A:
            case 0x36: return SHIFT;
            case 0x1D: return CTRL;
            case 0x38: return ALT;
        }
    }
    return 0;
}

void kb_handler(struct int_frame *r) {
    uint8_t sc, m;
    char ch;

    if ((inb(KB_STAT) & KB_STAT_OBF) == 0) {
        return;
    }

    sc = inb(KB_DATA);
    if (DEBUG == 1) {
        printf("kb_handler: sc: 0x%x\n", sc);
    }

    if (KB_IS_ESCAPE(sc)) {
        if (DEBUG == 1) {
            printf("kb_handler: ESCAPE\n");
        }
        kb_mode |= E0ESC;
        return;
    } 
    
    if ((m = shift(sc))) {
        if (KB_IS_RELEASE(sc)) {
            kb_mode &= ~m;
        } else {
            kb_mode |= m;
        }
        return;
    }

    if (kb_mode & SHIFT) {
        if (DEBUG == 1) {
            printf("kb_handler: SHIFT\n");
        }

        ch = kb_shift_map[sc & 0x7f];
    } else {
        ch = kb_map[sc & 0x7f];
    }

    if (kb_mode & CTRL) {
        if (DEBUG == 1) {
            printf("kb_handler: CTRL\n");
        }
        switch(ch) {
            case 'd': printf("kb_handler: EOF\n"); ch = TTY_EOF; break;
            case 'c': printf("kb_handler: INT\n"); ch = TTY_INT; break;
        }
    }

    if (kb_mode & ALT) {
        if (DEBUG == 1) {
            printf("kb_handler: ALT\n");
        }
    }

    if (KB_IS_RELEASE(sc)) {
        if (DEBUG == 1) {
            printf("kb_handler: release, clear E0ESC\n");
        }
        kb_mode &= ~E0ESC;
    } else if (ch != 0) {
        if (DEBUG == 1) {
            printf("kb_handler: char: [%c]\n", ch);
        }

        if (putch(ch) < 0) {
            printf("kb: buffer overflow.\n");
        }
    }
}

void keyboard_install() {
    irq_install_handler(1, kb_handler, 0x08, 0x8E);
}