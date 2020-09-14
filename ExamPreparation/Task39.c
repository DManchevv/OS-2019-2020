#include <err.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char** argv){
	
	if (argc != 4){
		errx(1, "Wrong arguments! Usage: %s <file1> <file2> <file3>", argv[0]);
	}
	
	int f1 = open(argv[1], O_RDONLY);
	if (f1 == -1){
		err(2, "Failed to open file %s", argv[1]);
	}
	int f2 = open(argv[2], O_RDONLY);
	if (f2 == -1){
		int olderrno = errno;
		close(f1);
		errno = olderrno;
		err(3, "Failed to open file %s", argv[2]);
	}
	struct stat st;
	if (stat(argv[1], &st) == -1){
		int olderrno = errno;
		close(f1);
		close(f2);
		errno = olderrno;
		err(4, "Failed stat with file %s", argv[1]);
	}	
	if (st.st_size == 0){
		int olderrno = errno;
        close(f1);
        close(f2);
        errno = olderrno;
		errx(5, "Empty file: %s", argv[1]);
	}
	if (st.st_size % sizeof(uint8_t) != 0){
		int olderrno = errno;
        close(f1);
        close(f2);
        errno = olderrno;
		errx(6, "Corrupted file: %s", argv[1]);
	}
	if (st.st_size > 0xFFFF){
		warnx("Size of %s is greater than 0xFFFF", argv[1]);
	}
	struct stat st2;
	if (stat(argv[2], &st2) == -1){
		int olderrno = errno;
        close(f1);
        close(f2);
        errno = olderrno;
		err(7, "Failed stat with file %s", argv[2]);
	}
	if (st2.st_size == 0){
		int olderrno = errno;
        close(f1);
        close(f2);
        errno = olderrno;
		errx(8, "Empty file: %s", argv[2]);
	}
	if (st2.st_size % sizeof(uint8_t) != 0){
		int olderrno = errno;
        close(f1);
        close(f2);
        errno = olderrno;
		errx(9, "Corrupted file: %s", argv[2]);
	}
	if (st2.st_size > 0xFFFF){
		warnx("Size of %s is greater than 0xFFFF", argv[2]);
	}
	if (st2.st_size != st.st_size){
		int olderrno = errno;
        close(f1);
        close(f2);
        errno = olderrno;
		errx(10, "Files do not have equal length!");
	}
	struct triple{
		uint16_t offset;
		uint8_t oldValue;
		uint8_t newValue;
	};
	
	int patch = open(argv[3], O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
	if (patch == -1){
		int olderrno = errno;
        close(f1);
        close(f2);
        errno = olderrno;
		err(11, "Failed to open file %s", argv[3]);
	}
	
	struct triple y;
	ssize_t read_size1;
	ssize_t read_size2;
	uint8_t num1;
	uint8_t num2;	

	while((read_size1 = read(f1, &num1, sizeof(num1))) == (ssize_t)(sizeof(num1)) && (read_size2 = read(f2, &num2, sizeof(num2))) == (ssize_t)(sizeof(num2))){
		if (num1 != num2){
			y.offset = lseek(f1, -1*sizeof(num1), SEEK_CUR);
			/*if (y.offset == -1){
				int olderrno = errno;
       			close(f1);
        		close(f2);
       			close(patch);
				errno = olderrno;
				err(12, "Lseek error!");
			}*/
			if (lseek(f1,1*sizeof(num1), SEEK_CUR) == -1){
				int olderrno = errno;
                close(f1);
                close(f2);
                close(patch);
                errno = olderrno;
                err(12, "Lseek error!");	
			}
			y.oldValue = num1;
			y.newValue = num2;
			if (write(patch, &y, sizeof(y)) != sizeof(y)){
				int olderrno = errno;
        		close(f1);
        		close(f2);
       			close(patch);
				errno = olderrno;
				err(13, "Failed to write in file %s", argv[3]);
			}
		}
	}
	if (read_size1 == -1 || read_size2 == -1){
		int olderrno = errno;
        close(f1);
        close(f2);
		close(patch);
        errno = olderrno;
		err(14, "Failed to read from files!");
	}
	close(f1);
	close(f2);
	close(patch);
	exit(0);
}
