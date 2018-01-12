#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char *errstr;
  int n = 0;
  for (n = 0; n < 4096; n++) {
    errstr = strerror(n);
    if (strstr(errstr, "Unknown") == NULL)
      printf("[%04d]: %s\n", n, errstr);
  }
}
