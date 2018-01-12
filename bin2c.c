/***************************
 * File: bin2c.c
 * Author: Guo Xiaoyong
 * Date: DEC 19, 2011
 ***************************/

#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *fp;
  char *filename = NULL;
  char *varname = NULL;
  int letter;
  size_t size;

  if (argc < 3) {
    fprintf(stderr, "too few input parameters!\n\n");
    printf("bin2c: convert a binary file to a large C array\n"
           "USAGE:\n"
           "   bin2c file_name variable_name\n\n");
    return 1;
  }
  filename = argv[1];
  varname = argv[2];
  fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "bin2c error: cannot open the file %s\n", filename);
    return 2;
  }

  size = 0;
  printf("unsigned char %s[] = { ", varname);
  while ((letter = fgetc(fp)) != EOF) {
    assert(letter >= 0 && letter <= 255);
    printf("0x%02X,\n", letter);
    size++;
  }
  printf("};\n\n");
  printf("/* The size of the array is %lu. */", size);
  fclose(fp);
  return 0;
}
