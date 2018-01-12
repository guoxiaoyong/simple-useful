#include <stdio.h>

char outbuf[BUFSIZ];
int main(void) {
  setbuf(stdout, outbuf);
  puts("This is a test of buffered output.\n");
  puts(outbuf);
  fflush(stdout);
  puts(outbuf);
  return 0;
}
