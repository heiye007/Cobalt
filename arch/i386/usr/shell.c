#include <i386/vga.h>
#include <i386/8042.h>
#include <string.h>
#include <stdint.h>

int lastlinedetect;

uint32_t shell()
{
  char c, line[80], cmd[80], arg[80];
  printf("Running Shell...\n");

  while(1)
  {
    printf("$ ");
    lastlinedetect = 0;
    memset(line, '\0', 80);
    memset(cmd, '\0', 80);
    memset(arg, '\0', 80);

    while((c = getch()) != '\n')
    {
      // cake: Well, backspacing 'works' using
      // nasty hacks, we can't rely on integer
      // comparison for checking if we 'arrived'
      // to the shell sign. We should use a buffer
      // so it stays as a known-bug without a proper fix.
      if (c == '\b')
      {
        if (lastlinedetect == 0) {
          continue;
        } else {
          if (get_row() == 0) {
            lastlinedetect--;
            update_cursor(79, get_col()-1);
            putch(' ');
            update_cursor(80, get_col()-1);
          } else {
            lastlinedetect--;
            update_cursor(get_row()-1, get_col());
            putch(' ');
        }
        }
      }
      else
      {
        line[lastlinedetect] = c;
        lastlinedetect++;
      }
      putch(c);
    }

    putch(c);
    line[lastlinedetect] = '\0';

    int pos = 0;

    while(line[pos] != ' ' && line[pos] != '\0') pos++;

    memcpy(cmd, line, pos);
    memcpy(arg, line + pos + 1, lastlinedetect - pos);
    cmd[pos] = '\0';
    arg[lastlinedetect - pos] = '\0';

    if (!strcmp(cmd, "hello"))
    {
      printf("Hello!\n");
    }
    else if (!strcmp(cmd, "panic"))
    {
      int a = 10;
      int b = 0;
      int c = a/b;
      printf("%d", c);
    }
    else
    {
      printf("Unknown command: %s\n", cmd);
    }
  }

  return 0;
}