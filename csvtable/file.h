#include <stdio.h>
#include <string.h>


inline
char* file_get_contents(const char* filename, char* contents) {


    FILE* fp;
    int c;
    int n;

    fp = fopen(filename, "rb");

    if (fp == NULL) {
        contents[0] = '\0';
        return contents;
    }

    while ( (c = fgetc(fp)) != EOF ) {

        *contents++ = (char)c;
    }

    fclose(fp);

    return contents;

}


inline
size_t file_put_contents( const char* filename,
                          const char* contents,
                          const size_t siz) {

    FILE* fp;

    fp = fopen(filename, "wb");

    if (fp == NULL) {

        return 0;
    }

    size_t sz = fwrite(contents, 1, siz, fp);
    fclose(fp);
    return sz;
}


