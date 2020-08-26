#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main (int argc, char* argv[]){
     
	if (argc < 3) {
		errx(1, "Please enter arguments");
		exit(-1);
	}

	int fd1;
	int fd2;
	char c;
	int i = 1;	

	while (i+1 < argc){
		if ((fd1 = open(argv[i], O_RDWR)) == -1){
			close(fd1);
			err(2, "%s", argv[i]);
			exit(-1);
		}

		char* destDir = malloc( strlen(argv[argc - 1]) + strlen(argv[i]));
               strcpy(destDir,argv[argc -1]);
               strcat(destDir,argv[i]);
		
		if ((fd2 = open(destDir, O_RDWR | O_CREAT)) == -1){
			close(fd1);
			close(fd2);
			err(3, "%s", destDir);
			exit(-1);
		}
						
		while(read(fd1, &c, 1)){
			write(fd2, &c, 1);
		}
		close(fd1);
		i++;
	}
	
	close(fd1);
	close(fd2);
	exit(0);
}

