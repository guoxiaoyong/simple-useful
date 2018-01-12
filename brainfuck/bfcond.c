#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_loop(char in, const char out, int tabbing)
{
    int i, lc, lm;

    lc = (int)in / 10;
    lm = (int)in % 10;

    if (lc > 0)
    {
        for (i = 0; i < tabbing; i++)
            printf("\t");

        printf("+++++ +++++\n");

        for (i = 0; i < tabbing; i++)
            printf("\t");

        printf("[\n");

        for (i = 0; i < tabbing; i++)
            printf("\t");

        printf("\t>");

        for (i = 0; i < lc; i++)
        {
            if (i % 5 == 0)
                printf(" ");
            putchar(out);
        }

        printf("\n");

        for (i = 0; i < tabbing; i++)
            printf("\t");

        printf("\t< -\n");

        for (i = 0; i < tabbing; i++)
            printf("\t");

        printf("]\n");

        for (i = 0; i < tabbing; i++)
            printf("\t");

        printf(">");
    }
    for (i = 0; i < lm; i++)
    {
        if (i % 5 == 0)
            printf(" ");
        putchar(out);;
    }
    printf(" <\n\n");
}

int main(int argc, char ** argv)
{
    char comp;
    int i;

    comp = argv[1][0];

    generate_loop(comp, '-', 0);
    printf("\n[\n\t>\n");
    generate_loop((char)'Y', '+', 1);
    printf(". <\n]\n");
    generate_loop((char)'Y', '-', 0);
    generate_loop(comp, '+', 0);
}


