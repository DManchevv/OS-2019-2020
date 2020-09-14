#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>

int main(int argc, char** argv){
	
	if (argc != 5){
		errx(1, "Wrong arguments! Usage: %s <file1 input> <file2 input> <file3 output> <file4 output>", argv[0]);
	}
	
	struct stat st;
	if (stat(argv[1], &st) == -1){
		err(2,"Failed stat with file: %s", argv[1]);
	}
	if (st.st_size == 0){
		errx(3, "Empty file!");
	}
	if (st.st_size % sizeof(uint8_t) != 0){
		errx(4, "File is corrupted!");
	}
	
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1){
		err(5, "Failed to open file %s", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1){
		int olderrno = errno;
		close(fd1);
		errno = olderrno;
		err(6, "Failed to open file %s", argv[2]);
	}
	
	int out1 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
	if (out1 == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(7, "Failed to open file %s", argv[3]);
	}
	
	int out2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
	if (out2 == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		close(out1);
		errno = olderrno;
		err(8, "Failed to open file %s", argv[4]);
	}

	struct str{
		uint16_t startPos;
		uint8_t length;
		uint8_t saved;
	};

	struct str s;
	ssize_t read_size;
	uint8_t num;
	ssize_t read_size2;
	uint8_t counter = 0;
	struct str input;
	while ((read_size = read(fd2, &s, sizeof(s))) == (ssize_t)(sizeof(s))){
			lseek(fd1, s.startPos, SEEK_SET);
			if ((read_size2 = read(fd1, &num, sizeof(num))) != (ssize_t)(sizeof(num))){
				int olderrno = errno;
				close(fd1);
				close(fd2);
				close(out1);
				close(out2);
				errno = olderrno;
				err(7, "Failed to read file %s", argv[1]);
			}
	
			if (0x41 <= num && num <= 0x5A){
				input.startPos = lseek(out1, 0, SEEK_CUR);
				write(out1, &num, sizeof(num));	
				counter = 1;
				while ((read_size2 = read(fd1, &num, sizeof(num))) == (ssize_t)(sizeof(num))){
					if (write(out1, &num, sizeof(num)) != sizeof(num)){
						err(8, "Failed to write in file %s", argv[3]);
					}
					counter++;
					if (counter == s.length) {
						input.length = s.length;
						input.saved = s.saved;
						if (write(out2, &input, sizeof(input)) != sizeof(input)){
							err(9, "Failed to write in file %s", argv[4]);
						}
						break;
					}
				}
				if (read_size2 == -1){
					int olderrno = errno;
					close(fd1);
					close(fd2);
					close(out1);
					close(out2);
					errno = olderrno;
					err(8, "Failed to read from file %s", argv[1]);
				}
			}
	}
	if(read_size == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);
		close(out1);
		close(out2);
		errno = olderrno;
		err(9, "Failed t oread from file %s", argv[2]);
	}
	close(fd1);
	close(fd2);
	close(out1);
	close(out2);
	exit(0);
}
