#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>

int main (int argc, char** argv){
	
	if (argc != 4){
		errx(1, "Wrong arguments! Usage: %s <file1> <file2> <file3>", argv[0]);
	}
	
	struct header {
		uint32_t magic;
		uint8_t header_version;
		uint8_t data_version;
		uint16_t count;
		uint32_t reserved1;
		uint32_t reserved2;
	};

	struct dataVersion0{
		uint16_t offset;
		uint8_t oldValue;
		uint8_t newValue;
	};
	
	struct dataVersion1{
		uint32_t offset;
		uint16_t oldValue;
		uint8_t newValue;
	};
		
	struct header hd;
	struct dataVersion0 v0;
	struct dataVersion1 v1;
	struct stat st;
	if (stat(argv[1], &st) == -1){
		err(2, "Failed stat with file %s", argv[1]);
	}
	if (st.st_size == 0){
		errx(3, "File %s is empty!", argv[1]);
	}
	int patch = open(argv[1], O_RDONLY);
	if (patch == -1){
		err(4, "Failed to open file %s", argv[1]);
	}
	
	ssize_t read_size;
	if ((read_size = read(patch, &hd, sizeof(hd))) != (ssize_t)(sizeof(hd))){
		int olderrno = errno;
		close(patch);
		errno = olderrno;
		err(5, "Failed to read from file %s", argv[1]);
	}
	int f1 = open(argv[2], O_RDONLY);
	if (f1 == -1){
		int olderrno = errno;
		close(patch);
		errno = olderrno;
		err(6, "Failed to open file %s", argv[2]);
	}
	int f2 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
	if (f2 == -1){
		int olderrno = errno;
		close(patch);
		close(f1);
		errno = olderrno;
		err(7, "Failed to open file %s", argv[3]);
	}
	 
	if (hd.data_version == 0x00){
		uint8_t num;
		struct stat st2;
		if (stat(argv[2], &st2) == -1){
			int olderrno = errno;
			close(patch);
			close(f1);
			close(f2);
			errno = olderrno;
			err(8, "Failed stat with file %s", argv[2]);
		}
		if (st2.st_size == 0){
			errx(9, "File %s is empty!", argv[2]);
		}
		if (st2.st_size % sizeof(uint8_t) != 0){
			errx(10, "File %s is corrupted!", argv[2]);
		}
		while ((read_size = read(f1, &num, sizeof(num))) == (ssize_t)(sizeof(num))){
			if (write(f2, &num, sizeof(num)) != (ssize_t)(sizeof(num))){
				int olderrno = errno;
            	close(patch);
				close(f1);
				close(f2);
            	errno = olderrno;
				err(11, "Failed to write in file %s", argv[3]);
			}
		}
		if (read_size == -1){
			int olderrno = errno;
            close(patch);
            close(f1);
            close(f2);
            errno = olderrno;
			err(12, "Failed to read from file %s", argv[2]);
		}
		while ((read_size = read(patch, &v0, sizeof(v0))) == (ssize_t)(sizeof(v0))){
			if (lseek(f2, v0.offset, SEEK_SET) == -1){
				int olderrno = errno;
                close(patch);
                close(f1);
                close(f2);
                errno = olderrno;
				err(13, "Lseek error!");
			}
			if (read(f2, &num, sizeof(num)) != (ssize_t)(sizeof(num))){
				int olderrno = errno;
                close(patch);
                close(f1);
                close(f2);
                errno = olderrno;
				err(14, "Failed to read from file %s", argv[3]);
			}
			if (num != v0.oldValue){
				int olderrno = errno;
                close(patch);
                close(f1);
                close(f2);
                errno = olderrno;
				err(15, "Byte is not written or doesn't exist!");
			}
			else {
				if (lseek(f2, -1*sizeof(num), SEEK_CUR) == -1){
					int olderrno = errno;
                	close(patch);
                	close(f1);
                	close(f2);
                	errno = olderrno;
					err(16, "Lseek error!");
				}
				if (write(f2, &v0.newValue, sizeof(v0.newValue)) != (ssize_t)(sizeof(v0.newValue))){
					int olderrno = errno;
                	close(patch);
                	close(f1);
                	close(f2);
                	errno = olderrno;
					err(17, "Failed to write in file %s", argv[3]);
				}
			}
		}
	}
	if (hd.data_version == 0x01){
		uint16_t num;
        struct stat st2;
        if (stat(argv[2], &st2) == -1){
            int olderrno = errno;
            close(patch);
            close(f1);
            close(f2);
            errno = olderrno;
            err(8, "Failed stat with file %s", argv[2]);
        }
        if (st2.st_size == 0){
            errx(9, "File %s is empty!", argv[2]);
        }
        if (st2.st_size % sizeof(uint16_t) != 0){
            errx(10, "File %s is corrupted!", argv[2]);
        }
        while ((read_size = read(f1, &num, sizeof(num))) == (ssize_t)(sizeof(num))){
            if (write(f2, &num, sizeof(num)) != (ssize_t)(sizeof(num))){
                int olderrno = errno;
                close(patch);
                close(f1);
                close(f2);
                errno = olderrno;
                err(11, "Failed to write in file %s", argv[3]);
            }
        }
        if (read_size == -1){
            int olderrno = errno;
            close(patch);
            close(f1);
            close(f2);
            errno = olderrno;
            err(12, "Failed to read from file %s", argv[2]);
        }
        while ((read_size = read(patch, &v1, sizeof(v1))) == (ssize_t)(sizeof(v1))){
            if (lseek(f2, v1.offset, SEEK_SET) == -1){
                int olderrno = errno;
                close(patch);
                close(f1);
                close(f2);
                errno = olderrno;
                err(13, "Lseek error!");
            }
            if (read(f2, &num, sizeof(num)) != (ssize_t)(sizeof(num))){
                int olderrno = errno;
                close(patch);
                close(f1);
                close(f2);
                errno = olderrno;
                err(14, "Failed to read from file %s", argv[3]);
            }
            if (num != v1.oldValue){
                int olderrno = errno;
                close(patch);
                close(f1);
                close(f2);
                errno = olderrno;
                err(15, "Byte is not written or doesn't exist!");
            }
            else {
                if (lseek(f2, -1*sizeof(num), SEEK_CUR) == -1){
                    int olderrno = errno;
                    close(patch);
                    close(f1);
                    close(f2);
                    errno = olderrno;
                    err(16, "Lseek error!");
                }
                if (write(f2, &v1.newValue, sizeof(v1.newValue)) != (ssize_t)(sizeof(v1.newValue))){
                    int olderrno = errno;
                    close(patch);
                    close(f1);
                    close(f2);
                    errno = olderrno;
                    err(17, "Failed to write in file %s", argv[3]);
                }
            }
        }	
	}
	close(patch);
	close(f1);
	close(f2);
	exit(0);
}
