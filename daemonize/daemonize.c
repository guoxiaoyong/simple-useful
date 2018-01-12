#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {

    daemon(1,1);
    execv(argv[1], argv+1);
    return 0;
}
