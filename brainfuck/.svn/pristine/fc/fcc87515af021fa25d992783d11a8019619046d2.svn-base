#include<stdio.h>

int main(int argc, char* argv[]) {

    if (argc != 3) return 0;

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    printf("a = %d, b = %d\n", a, b);

    int flag=0;
    int eq=0;
    int bsmall = 0;

    while (a) {

        flag=1;
        eq= b;
        while (eq) {
            eq= 0;
            flag--;
        }


        a--;

        while (flag) {

            a = 0;
            b++; // not necessary
            bsmall = 1;
            flag = 0;
            printf("runed!\n");
        }

        b--;
    }

    /*
    eq = 0;
    flag = 1;
    if (bsmall == 0 && b == 0) eq = 1;
    */

    eq = bsmall+b;
    printf("temp eq = %d\n", eq);
    flag = 2;
    while (eq) {
        eq = 0;
        flag = 0;
    }

    bsmall = bsmall+flag;

    /*
    printf("a > b = %d\n", bsmall);
    printf("eq = %d\n", eq);
    */


    if (bsmall == 0) {
         printf("a < b\n");
    } else if (bsmall == 1) {
         printf("a > b\n");
    } else if (bsmall == 2) {
         printf("a == b\n");
    } else {
         fprintf(stderr, "wrong!\n");
    }

    printf("a = %d, b = %d\n", a, b);

    return 0;
}

