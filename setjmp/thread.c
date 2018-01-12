#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

int NUM_ENTITY;
int num;
jmp_buf mainctx;
jmp_buf childctx[20];

void call_with_cushion(int);

void child(int id) {

  int n = 0;

  setjmp(childctx[id]);
  while (1) {

    if (num != id) {

      printf("%02d: num = %d\n", id, id);
      num = id;
    }

    if (id < NUM_ENTITY && n == 0) {

      n = 1;
      call_with_cushion(id + 1);
      // child(id+1);
    }

    if (!setjmp(childctx[id]))
      longjmp(mainctx, id);
  }
}

void call_with_cushion(int id) {
  char space[1000];
  space[999] = 1;
  child(id);
}

int main(int argc, char *argv[]) {

  int m, n, k;

  if (argc != 2)
    return 0;

  NUM_ENTITY = atoi(argv[1]);

  if (!setjmp(mainctx)) {

    call_with_cushion(1);
  }

  while (1) {

    m = setjmp(mainctx);

    if (m != 0)
      k = m;

    if (!m) {
      while ((n = (rand() % NUM_ENTITY) + 1) == k)
        ;
      printf("n = %d\n", n);
      longjmp(childctx[n], 1);
    }
  }

  return 0;
}
