// Create your own foo.txt file in the same directory.
// NOTE: It is not necessary to create bar.txt, the flag O_CREAT will create it automatically for you if bar.txt is not found in the file tree.

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
	int fd1;
	int fd2;
	char c;
	if ((fd1 = open("foo.txt", O_RDONLY)) == -1 ) {
		write(2, "File failed to open in read mode\n", 33);
		exit(-1);
	}

	if ((fd2 = open("bar.txt", O_CREAT|O_WRONLY)) == -1 ) {
		write(2, "File failed to open in write mode\n", 34);
		close(fd1);
		exit(-1);
	}

	while (read(fd1, &c, 1)){
		write(fd2, &c, 1);
	}

	close(fd1);
	close(fd2);
	exit(0);
}
