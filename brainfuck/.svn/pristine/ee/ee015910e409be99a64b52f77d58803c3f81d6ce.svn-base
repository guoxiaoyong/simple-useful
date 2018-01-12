#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    int dd=0, r=0, dr=0, q=0;

    if (argc != 3) return 0;

    dd = atoi(argv[1]);
    dr = atoi(argv[2]);

    printf("divident = %d\n", dd);
    printf("divisor = %d\n",  dr);

    int tmp = 0;
    int dr_zero_flag = 0;

    while (dd) {

        dd--;
        dr--;
        r++;
        dr_zero_flag++;

        tmp=dr;

        while (tmp) {
            tmp--;
            dr_zero_flag=0;
        }

        while (dr_zero_flag) {
            dr=r;
            r=0;
            q++;
            dr_zero_flag--;
            printf("%d\n", dd);
        }

        /*
        if (dr == 0) {

            dr = r;
            r = 0;
            q++;
        }
        */
    }


    printf("q = %d\n", q);
    printf("r = %d\n", r);

    return 0;
}

