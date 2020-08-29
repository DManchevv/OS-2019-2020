#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
struct content_t {
        uint16_t pos;
        uint8_t orig;
        uint8_t change;
};

int main(int argc, char** argv){

	if (argc != 4) {
		errx(1, "Usage: %s patch.bin f1.bin f2.bin", argv[0]);
	}
	
	struct stat st;
	
	if(stat(argv[1], &st) == -1){
		err(2, "Fail using stat with file %s", argv[1]);
	}

	if (st.st_size % sizeof(struct content_t) != 0){
		err(3, "Invalid size of file %s", argv[1]);
	}
	
	if (stat(argv[2], &st) == -1){
		err(4, "Fail using stat with file %s", argv[2]);
	}

	if (st.st_size > 0xFFFF){
		warnx("%s file size greater than 0xFFFF", argv[2]);
	}

	int fd1;
	int fd2;
	int fd3;

	if ((fd1 = open(argv[1], O_RDONLY)) == -1){
		err(5, "Error opening file %s", argv[1]);
	}

	if ((fd2 = open(argv[2], O_RDONLY)) == -1){
		int olderrno = errno;
		close(fd1);
		errno = olderrno;
		err(6, "Error opening file %s", argv[2]);
	}

	if ((fd3 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC | S_IRUSR | S_IWUSR)) == -1){
		int olderrno = errno;
		close(fd1);
		close(fd2);	
		errno = olderrno;
		err(7, "Error opening file %s", argv[3]);
	}
	
	uint8_t num;
	ssize_t read_size;
	ssize_t write_size;

	while((read_size = read(fd2, &num, sizeof(num))) > 0 &&((write_size = write(fd3, &num, sizeof(num))) > 0)){ }

	if ((read_size != 0) || (write_size != 0)) {
		int olderrno = errno;
		close(fd1);
		close(fd2);
		close(fd3);
		errno = olderrno;
		err(8, "Error reading/writing %s -> %s", argv[2], argv[3]);
	}

	close(fd2);
	
	struct content_t c;
	while(read(fd3, &c, sizeof(c)) == sizeof(c))
	{
		if(lseek(fd3, c.pos, SEEK_SET) == -1){
			int olderrno = errno;
			close(fd1);
			close(fd3);
			errno = olderrno;
			err(9, "Cannot seed to %d in %s", c.pos, argv[3]);
		}
		read(fd3, &num, sizeof(num));
		if(num == c.orig){
			if (lseek(fd3, -1, SEEK_CUR) == -1){
				int olderrno = errno;
				close(fd1);
				close(fd3);
				errno = olderrno;
				err(10, "Cannot seek to %d in %s", c.pos, argv[3]);
	     	}
			write(fd3, &(c.change), sizeof(c.change));
		}
		else {
			int olderrno = errno;
			close(fd1);
			close(fd3);
			errno = olderrno;
			errx(11, "There is no such byte in %s", argv[3]);
		}
	}

	close(fd1);
	close(fd3);
	exit(0);
}
