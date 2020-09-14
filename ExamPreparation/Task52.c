#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main (int argc, char** argv){
	
	char cmd[5];
	if (argc == 2){
		strcpy(cmd, argv[1]);
	}
	else {
		strcpy(cmd, "echo");
	}
	char test;
	int index = 0;
	char buf[10];
	char buf2[10];
	while(read(0, &test, sizeof(test)) == (ssize_t)(sizeof(test))){
		if(test == '\n') break;
		char c;
		int counter = 0;
		if (index % 2 == 0) buf[counter] = test;
		if (index % 2 != 0) buf2[counter] = test;
		counter++;
		while(read(0, &c, sizeof(c)) == (ssize_t)(sizeof(c))){
			if (c == '\n' || c == ' ') break;
			if (index % 2 == 0) buf[counter] = c;
			if (index % 2 != 0) buf2[counter] = c;
			counter++;
		}
		if (index % 2 == 0) buf[counter] = '\0';
		if (index % 2 != 0) buf2[counter] = '\0';
		if (index % 2 == 0 && strlen(buf) > 4){
			errx(1, "The length of the string is more than 4 characters! Terminating.");
		}
		if (index % 2 != 0 && strlen(buf2) > 4){
			errx(2, "The length of the string is more than 4 characters! Terminating.");
		}
		if (index % 2 != 0){
			pid_t p = fork();
			if (p < 0){
				err(3, "Fork error!");
			}
			else if (p == 0){
				if (execlp(cmd,cmd, buf, buf2, (char *) NULL) == -1){
					err(4, "Exec error with command %s", cmd);
				}
			}
			wait(NULL);
		}
		index++;
	}
	if (index % 2 != 0){
		pid_t p = fork();
		if (p < 0){
			err(5, "Fork error!");
		}
		else if (p == 0){
			if (execlp(cmd,cmd, buf, (char *) NULL) == -1){
				err(6, "Exec error with command %s", cmd);
			}
		}
		wait(NULL);
	}
	exit(0);
}
