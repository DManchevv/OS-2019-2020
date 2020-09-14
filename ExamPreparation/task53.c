#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv){
	
	if (argc != 2){
		errx(69, "Wrong arguments! Usage: %s <dir>.",argv[0]);
	}

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
		if ((execlp("find", argv[1] , "-type", "f"  , "-mtime" , "-100" , "-ls", (char *) NULL)) == -1){
			err(4, "Exec error with command find!");
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
		if ((execlp("tr", "tr",  "-s", "[:blank:]", (char *) NULL)) == -1){
			err(10, "Exec error with command tr!");
		}
	}
	if (wait(NULL) == -1){
		err(11, "Could not wait second child!");
	}
	close(b[1]);
	int c[2];
	if (pipe(c) < 0){
		err(12, "Could not create pipe!");
	}
	
	pid_t p3 = fork ();
	if (p3 < 0){
		err(13, "Fork error!");
	}
	else if (p3 == 0){
		if ((dup2(b[0],0)) == -1){
			err(12, "Could not create duplicate!");
		}
		close(c[0]);
		if ((dup2(c[1],1)) == -1){
			err(13, "Could not create duplicate!");
		}
		if ((execlp("cut", "cut", "-d", " ", "-f9-", (char *) NULL)) == -1){
			err(14, "Exec error with command cut!");
		}
	}
	if (wait(NULL) == -1){
		err(15, "Could not wait third child!");
	}
	close(c[1]);
	int d[2];
	if (pipe(d) < 0){
		err(16, "Could not create pipe!");
	}
	pid_t p4 = fork();
	if (p4 < 0){
		err(17, "Fork error!");
	}
	else if (p4 == 0){
		if ((dup2(c[0],0)) == -1){
			err(18, "Could not create duplicate!");
		}
		close(d[0]);
		if ((dup2(d[1], 1)) == -1){
			err(19, "Could not create duplicate!");
		}
		if ((execlp("sort","sort", (char *) NULL)) == -1){
		err(20, "Exec error with command sort!");
		}
	}
	if (wait(NULL) == -1){
		err(21, "Could not wait for forth child!");
	}
	close(d[1]);
	int e[2];
	if (pipe(e) < 0){
		err(22, "Could not create pipe!");
	}
	
	pid_t p5 = fork();
	if (p5 < 0){
		err(23, "Fork error!");
	}
	else if (p5 == 0){
		if ((dup2(d[0],0)) == -1){
			err(24, "Could not create duplicate!");
		}
		close(e[0]);
		if ((dup2(e[1],1)) == -1){
			err(25, "Could not create duplicate!");
		}
		if ((execlp("tail", "tail", "-n", "1", (char *) NULL)) == -1){
			err(26, "Exec error with command tail!");
		}
	}
	if (wait(NULL) == -1){
		err(27, "Could not wait for fifth child!");
	}
	close(e[1]);
	if ((dup2(e[0],0)) == -1){
		err(28, "Could not create a duplicate!");
	}
	if ((execlp("cut","cut","-d", " ", "-f4", (char *) NULL)) == -1){
		err(29, "Exec error with command cut!");
	}
	exit(0);	
}
