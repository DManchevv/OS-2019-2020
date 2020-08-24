#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char* argv[]) {
	int fd1;
	int lines = 0;
	int words = 0;
	int bytes = 0;
	char c;
	if (argc != 2) {
		write(2, "err\n", 4);
		exit(-1);
	}

	if ((fd1 = open(argv[1], O_RDONLY)) == -1){
		write(2, "File failed to open in read mode\n", 33);
		exit(-1);
	}

	while (read(fd1, &c, 1)) {
		if (c == '\n') {
			lines = lines + 1;
			words = words + 1;
		}

		if (c == ' ') {
			words = words + 1;
		}

		bytes = bytes + 1;
	}
	printf ("%d %d %d\n", lines, words, bytes);
	close(fd1);
	exit(0);
}
