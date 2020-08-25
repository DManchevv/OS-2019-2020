// Create the command cp, using 2 arguments

#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h> 
             
int main (int argc, char* argv[]){

    if (argc != 3) {
        errx(2, "There must be two params\n");
    }
    
    int fd1;
    char c;
    if ((fd1 = open(argv[1], O_RDWR)) == -1){
        close(fd1);
        err(3, "%s", argv[1]);
    }

    int fd2 = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR, S_IWUSR);
    if (fd2 == -1){
        close(fd1);
        close(fd2);
        err(4, "%s", argv[2]);
    }

    while (read(fd1, &c, 1)){
        write(fd2, &c, 1);
    }

    close(fd1);
    close(fd2);
    exit(0);
}
