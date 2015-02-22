#include "buffer.h"
#include "definitions.h"

int loc = 0;
char buff[100];
int getc() {
  if (loc == 0)
    return getchar();
  return buff[--loc];
}

/* like getc, but skips spaces */
int getcs() {
	char c;
	while ((c = getc()) != ' ')
		;
	return c;
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
