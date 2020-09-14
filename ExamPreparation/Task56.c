#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char** argv){
		
	if (argc != 3){
		errx(1, "Wrong arguments! Usage: %s <file1> <file2>", argv[0]);
	}
	
	struct stat st;
	if (stat(argv[1], &st) == 0){
		err(2, "Failed stat with file %s", argv[1]);
	}
	if (st.st_size == 0){
		errx(3, "File %s is empty!", argv[1]);
	}
	if (st.st_size % sizeof(uint8_t) != 0){
		err(4, "File %s is corrupted!", argv[1]);
	}
	int output = open(argv[2], O_CREAT | O_RDWR | O_TRUNC , S_IRUSR, S_IWUSR);
	if (output == -1){
		err(6, "Failed to open file %s", argv[2]);
	}
	int a[2];
	if (pipe(a) < 0){
		int olderrno = errno;
		close(output);
		errno = olderrno;
		err(7, "Could not create pipe!");
	}
	pid_t p = fork();
	if (p < 0){
		int olderrno = errno;
		close(output);
		errno = olderrno;
		err(8, "Fork error!");
	}
	if (p == 0){
		close(a[0]);
		dup2(a[1],1);
		if (execlp("cat","cat",argv[1], (char *) NULL) == -1){
			int olderrno = errno;
			close(output);
			errno = olderrno;
			err(9, "Exec error with command cat!");
		}
	}
	wait(NULL);		//TODO error
	close(a[1]);
	dup2(a[0],0);
	uint8_t num;
	ssize_t read_size;
	int next = 0;
	uint8_t orig_num;
	while ((read_size = read(0, &num, sizeof(num))) == (ssize_t)(sizeof(num))){
		if (next == 1){
			if (num == (0x7D^0x20) || num == (0x00^0x20) || num == (0x55^0x20) || num == (0xFF^0x20)){
				orig_num = num^0x20;
				next = 0;
				write(output, &orig_num, sizeof(orig_num)); //TODO error
			}
		}
		if (num == 0x7D) next = 1;
		else next = 0;
	}
	close(output);
	exit(0);
}
