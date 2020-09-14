#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv){
	
	while(1){
		if ((write(1, "Mitko's prompt: ", 16)) != 16){
			err(1, "Could not write the prompt name!");
		}
		
		char cmd[10];
		ssize_t read_size;
		if ((read_size = read(0, &cmd, sizeof(cmd))) == -1){
			err(2, "Error while reading command from STDIN!");
		}
		cmd[read_size - 1] ='\0';
				
		if (strcmp("exit",cmd) == 0){
			break;
		}
		pid_t p = fork();
		if (p < 0){
			warn("Fork error for %s", cmd);
		}
		else if (p == 0){
			if ((execlp(cmd, cmd, (char *) NULL)) == -1){
				err(3, "Exec error with command: %s.", cmd);
			}
		}
		if (wait(NULL) == -1){
			err(4, "Could not wait %s", cmd);
		}
	}
	exit(0);
}
