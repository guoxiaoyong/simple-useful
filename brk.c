#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int judge(int num) {
  int i;
  for (i = 2; i < num; i++) {
    if (num % i == 0)
      return 0;
  }
  return 1;
}

int main() {
  int *p;
  void *old;
  int i;

  old = sbrk(0);
  if (old == -1)
    printf("sbrk error\n"), exit(-1);
  p = (int *)old;

  for (i = 1; i <= 1000; i++) {
    if (judge(i) == 1) {
      p = sbrk(4); // int类型所以又移4个字节
      if (p == -1)
        printf("brk error\n"), exit(-1);
      *p = i;
    }
  }

  p = (int *)old;
  while (p != sbrk(0)) {
    printf("%d ", *p);
    p++;
  }

  if (brk(old) == -1) //输出完毕，收回空间
    printf("brk error\n") exit(-1);
}
