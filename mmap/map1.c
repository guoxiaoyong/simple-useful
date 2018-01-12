#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

typedef struct people {
    char name[4];
    int age;
} people;


// map a normal file as shared mem:
int main(int argc, char** argv) {

    printf("page size = %d\n", sysconf(_SC_PAGE_SIZE));

    int fd = open(argv[1], O_CREAT|O_RDWR|O_TRUNC, 00777);

    lseek(fd, sizeof(people)*5-1, SEEK_SET);
    write(fd, "", 1);

    struct people* p_map = (people*)mmap(NULL, sizeof(people)*10, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);


    char temp = 'a';
    int i;
    for(i = 0; i < 10; i++) {
        temp += 1;
        memcpy((*(p_map+i)).name, &temp, 2);
        (*(p_map+i)).age = 20+i;
    }

    printf("initialize over\n");
    sleep(1);

    munmap(p_map, sizeof(people)*10);
    printf("umap ok \n");
    return 0;
}


