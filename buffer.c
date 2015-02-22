#include "buffer.h"

int loc = 0;
char buff[100];
int getc() {
  if (loc == 0)
    return getchar();
  return buff[--loc];
}

void ungetc(char c) {
  buff[loc++] = c;
}

void debug_buff() {
  int ptr = loc;
  puts("the buffer is:");
  for( ;ptr >= 0;ptr--)
    putchar(buff[ptr]);
}
