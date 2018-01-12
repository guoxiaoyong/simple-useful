#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct bfvm {

    unsigned char  mem[30000];
    unsigned char* tp;  // the pointer;

    char* code;
    size_t curr_ins;
};


struct bfvm* bfvm_init() {

    struct bfvm* bv = calloc(sizeof(struct bfvm), 1);

    if (bv == NULL) return NULL;

    bv->tp = bv->mem;
    bv->code = NULL;
    bv->curr_ins = 0;

    return bv;
}


void bfvm_destroy(struct bfvm* bv) {

    free(bv->code);
    free(bv);
}

void bfvm_load(struct bfvm* bv, FILE* fp) {

    fseek(fp, 0, SEEK_END);
    long n = ftell(fp);
    rewind(fp);

    bv->code = (char*)malloc(n+1+1000);
    if (bv->code == NULL) {

        perror("malloc failed!\n");
        exit(0);
    }

    bv->code[n+1] = 0;

    int c;
    n = 0;
    while ( (c = fgetc(fp)) != EOF ) {

        switch (c) {

        case '+':
        case '-':
        case '>':
        case '<':
        case '.':
        case ',':
        case '[':
        case ']':
        case '@':
        case '$':
            bv->code[n++] = (char)c;
            break;
        default:
            break;
        }
    }

    fclose(fp);
    bv->curr_ins = 0;
}

size_t bfvm_find_matching_rbrace(struct bfvm* bv) {

    size_t n;
    assert(bv->code[bv->curr_ins] == '[');

    int umatch = 1;
    n = bv->curr_ins+1;
    while ( bv->code[n] != 0 && umatch != 0) {

        if ( bv->code[n] == '[' ) {
            umatch++;
        } else if (bv->code[n] == ']') {
            umatch--;
        }
        n++;
    }

    return n-1;
}

size_t bfvm_find_matching_lbrace(struct bfvm* bv) {


    size_t n;
    assert(bv->code[bv->curr_ins] == ']');

    int umatch = 1;
    n = bv->curr_ins-1;
    while ( bv->code[n] != 0 && umatch != 0) {

        if ( bv->code[n] == ']' ) {
            umatch++;
        } else if (bv->code[n] == '[') {
            umatch--;
        }
        n--;
    }

    return n+1;
}


void bfvm_run(struct bfvm* bv) {

    int c;
    int n;
    int stop = 0;

#ifdef DEBUG_MSG
    int ni = 0;
#endif

#define print_state fprintf(stdout, "%04d: %c, tp = %ld, *tp= %d\n", ni++, bv->code[bv->curr_ins-1], (long)bv->tp-(long)bv->mem, *bv->tp);

    while ( bv->code[bv->curr_ins] != 0 )  {

        if (stop) break;

        switch ( bv->code[bv->curr_ins] ) {

        case '+':
            (*bv->tp)++;
            bv->curr_ins++;
#ifdef DEBUG_MSG
            print_state;
#endif
            break;

        case '-':
            (*bv->tp)--;
            bv->curr_ins++;

#ifdef DEBUG_MSG
            print_state;
#endif
            break;

        case '>':
            bv->tp++;
            bv->curr_ins++;
#ifdef DEBUG_MSG
            print_state;
#endif
            break;

        case '<':
            bv->tp--;
            bv->curr_ins++;
#ifdef DEBUG_MSG
            print_state;
#endif
            break;

        case '.':
            putchar(*bv->tp);
            bv->curr_ins++;
            fflush(stdout);

#ifdef DEBUG_MSG
            print_state;
#endif
            break;

        case ',':
            c = getchar();
            if (c != EOF) {
                *bv->tp = c;
            } else {
                *bv->tp = 0;
            }
            bv->curr_ins++;

#ifdef DEBUG_MSG
            print_state;
            fprintf(stdout, "%04d: , c = %d\n", ni++, c);
#endif
            break;

        case '[':
            if ( (*bv->tp) == 0 ) {
                bv->curr_ins = bfvm_find_matching_rbrace(bv)+1;
            } else {
                bv->curr_ins++;
            }

#ifdef DEBUG_MSG
            print_state;
#endif
            break;

        case ']':
            if ( (*bv->tp) == 0 ) {
                bv->curr_ins++;
            } else {
                bv->curr_ins = bfvm_find_matching_lbrace(bv)+1;
            }
#ifdef DEBUG_MSG
            print_state;
#endif
            break;

        case '@':

            for (n = 0; n < 20; n++) {
                    printf("m[%02d] = %03d\n", n, bv->mem[n]);
            }
            printf("tp = %d\n", (long)bv->tp-(long)bv->mem);
            bv->curr_ins++;

#ifdef DEBUG_MSG
            print_state;
#endif

            break;

        case '$':

            stop = 1;
            break;


        default:
            fprintf(stderr, "error!\n");
            return;
        }
    }
}


int main(int argc, char* argv[]) {

    FILE* fp;
    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "rb");
    }

    if (fp == NULL) return 0;

    struct bfvm* bv = bfvm_init();
    bfvm_load(bv, fp);
    bfvm_run(bv);
    bfvm_destroy(bv);


    return 0;
}


