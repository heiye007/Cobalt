unsigned int shell() {
  char c, line[80], cmd[80], arg[80];
  int i;
  printf("Running Shell...\n");

  while(1) {
    printf("$ ");
    i = 0;
    memset(line, '\0', 80);
    memset(cmd, '\0', 80);
    memset(arg, '\0', 80);
    while((c = getch()) != '\n') {
      if (c == '\b') {
        if (i == 0) continue;
        i--;
      } else {
        line[i] = c;
        i++;
      }
      putch(c);
    }
    putch(c);
    line[i] = '\0';

    int pos=0;
    while(line[pos] != ' ' && line[pos] != '\0') pos++;

    memcpy(cmd, line, pos);
    memcpy(arg, line + pos + 1, i - pos);
    cmd[pos] = '\0';
    arg[i - pos] = '\0';
    if (!strcmp(cmd, "hello")) {
      printf("Hello!\n");
    } else if (!strcmp(cmd, "panic")) {
      int a = 0;
      printf(80/a);
    } else {
      printf("Unknown command: %s\n", cmd);
    }
  }

  return 0;
}