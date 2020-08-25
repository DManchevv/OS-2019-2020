// Create cat command with only 1 argument

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    if (argc != 2){
        errx(2, "Please enter an argument");
        exit(-1);
    }

    int fd1;
    char c;

    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1){
        close(fd1);
        err(3, "%s", argv[1]);
        exit(-1);   
    }
    
    while(read(fd1, &c, 1)) {
        printf("%s", &c);
    }
    printf("\n");
    close(fd1);
    exit(0);
}
