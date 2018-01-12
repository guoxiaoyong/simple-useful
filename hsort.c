#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*compare_t)(const void *, const void *);

static void swap(void *A, void *B, size_t size) {
  char T[size];
  memcpy(T, A, size);
  memcpy(A, B, size);
  memcpy(B, T, size);
}

static void sift_down(void *base, size_t nmemb, size_t size, compare_t compar,
                      size_t n) {
  char *const BASE = (char *)base;
  char *parent;
  char *left;
  char *right;
  char *big_child;

  assert(nmemb > 1);
  const size_t last_parent = ((nmemb - 2) / 2);

  while (n <= last_parent) {
    parent = BASE + n * size;
    if (2 * n + 2 < nmemb) {
      left = BASE + size * (2 * n + 1);
      right = left + size;
      if (compar(left, right) < 0) {
        n = 2 * n + 2;
        big_child = right;
      } else {
        n = 2 * n + 1;
        big_child = left;
      }
    } else if (2 * n + 1 < nmemb) {
      left = BASE + size * (2 * n + 1);
      big_child = left;
      n = 2 * n + 1;
    } else {
      printf("never reach here!\n");
      break;
    }

    if (compar(parent, big_child) < 0) {
      swap(parent, big_child, size);
    } else {
      break;
    }
  }
}

static void heapify(void *base, size_t nmemb, size_t size, compare_t compar) {
  int start = (int)((nmemb - 2) / 2);
  while (start >= 0) {
    sift_down(base, nmemb, size, compar, start);
    start--;
  }
}

// ascending order
void hsort(void *base, size_t nmemb, size_t size, compare_t compar) {
  char *const BASE = (char *)base;
  char *END;
  if (nmemb == 0)
    return;

  heapify(base, nmemb, size, compar);
  END = BASE + (nmemb - 1) * size;
  swap(BASE, END, size);
  nmemb--;

  while (nmemb > 1) {
    sift_down(BASE, nmemb, size, compar, 0);
    END = BASE + (nmemb - 1) * size;
    swap(BASE, END, size);
    nmemb--;
  }
}

/*===================================================================*/
int Compare(const void *A, const void *B) {
  int a = *(int *)A;
  int b = *(int *)B;
  return (a >= b) - (a <= b);
}

int compare(const void *A, const void *B) {
  static int num;
  num++;
  int a = *(int *)A;
  int b = *(int *)B;
  printf("num = %d\n", num);
  return (a >= b) - (a <= b);
}

#include <stdio.h>

int main(int argc, char *argv[]) {
  int a[100] = {6, 5, 3, 1, 8, 7, 2, 4};
  size_t sz = sizeof(a) / sizeof(a[0]);
  size_t n;
  for (n = 0; n < sz; n++) {
    a[n] = rand();
  }

  if (argc != 2)
    return 0;

  printf("sz = %d\n", sz);
  if (argv[1][0] == 'q') {
    qsort(a, sz, sizeof(int), compare);
    printf("quick sort!\n");
  } else {
    hsort(a, sz, sizeof(int), compare);
    printf("heap sort!\n");
  }

  for (n = 0; n < sizeof(a) / sizeof(a[0]); n++) {
    printf("%d\n", a[n]);
  }

  return 0;
}
