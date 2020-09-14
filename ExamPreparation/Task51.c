#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv){
	
	int a[2];
	if (pipe(a) < 0){
		err(1, "Could not create pipe!");
	}
	
	pid_t p1 = fork();

	if (p1 < 0){
		err(2, "Fork error!");
	}
	else if (p1 == 0){
		close(a[0]);
		if ((dup2(a[1],1)) == -1){
			err(3, "Cannot create duplicate!");
		}
		if ((execlp("cut", "cut" , "-d" , ":" , "-f7" , "/etc/passwd", (char *) NULL)) == -1){
			err(4, "Exec error with command cut!");
		}
	}
	
	if (wait(NULL) == -1){
		err(5, "Could not wait first child!");
	}
	close(a[1]);
	int b[2];
	if (pipe(b) < 0){
		err(6, "Could not create pipe!");
	}

	pid_t p2 = fork();
	if (p2 < 0){
		err(7, "Fork error!");
	}
	else if (p2 == 0){
		if ((dup2(a[0],0)) == -1){
			err(8, "Cannot create duplicate!");
		}
		close(b[0]);
		if ((dup2(b[1],1)) == -1){
			err(9, "Cannot create duplicate!");
		}
		if ((execlp("uniq", "uniq", "-c", (char *) NULL)) == -1){
			err(10, "Exec error with command uniq!");
		}
	}
	if (wait(NULL) == -1){
		err(11, "Could not wait second child!");
	}
	close(b[1]);
	if ((dup2(b[0],0)) == -1){
		err(12, "Could not create duplicate!");
	}
	if ((execlp("sort", "sort", "-n", (char *) NULL)) == -1){
		err(13, "Exec error with command sort!");
	}
	exit(0);
}
