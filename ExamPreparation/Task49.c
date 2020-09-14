#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv){
	
	if (argc != 2) {
		errx(1,"Wrong arguments! Usage: %s <file>", argv[0]);
	}
	
	int a[2];
	
	if (pipe(a) == -1){
		err(2,"Could not create pipe!");
	}
	
	pid_t p1 = fork();

	if (p1 < 0){
		err(3,"Fork error!");
	}
	else if (p1 == 0){
		close(a[0]);
		if((dup2(a[1],1)) < 0){
			err(4,"Cannot create duplicate!");
		}
		if((execlp("cat","cat",argv[1],(char *) NULL)) == -1){
			err(5,"Exec error with command cat!");
		}
	}
	wait(NULL);
	close(a[1]);

	if((dup2(a[0],0)) < 0){
		err(5, "Could not create duplicate!");
	}
	
	if((execlp("sort", "sort", (char *) NULL)) == -1){
		err(6, "Exec error with command sort!");
	}
	
	exit(0);
}
