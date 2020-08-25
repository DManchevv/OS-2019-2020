// Create cat command with random number of arguments.

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	if (argc == 1){
		errx(2, "Please enter arguments");
		exit(-1);
	}

	int fd[100];
	int i = 0;
	char c;
	while (i+1 < argc){
		fd[i] = open(argv[i+1], O_RDONLY);
		
		if (fd[i] == -1){
			while (i > 0){
				close(fd[i]);
				i--;
			}
			err(3, "%s", argv[i+1]);
			exit(-1);
			break;
		}
		
		while (read(fd[i], &c, 1)){
			write(1,&c,1);
		}
		i++;
	}
	i--;
	while (i != 0) {
		close(fd[i]);
		i--;
	}
	exit(0);
}
