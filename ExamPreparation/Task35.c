// NOTE: Same task as task6.c in c_training directory.

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>

int main (int argc, char** argv){
	if (argc != 2){
		errx(1, "Please type 1 argument.");
		exit(-1);
	}

	int fd = open(argv[1], O_RDONLY);

	if (fd == -1){
		err(2, "Failed to open file in read mode: %s", argv[1]);
		exit(-1);
	}

	struct stat st;

	if (stat(argv[1],&st) == -1){
		err(3, "Failed to use stat with file: %s", argv[1]);
		exit(-1);
	}

	if (st.st_size == 0){
		err(4, "File is empty!");
		exit(0);
	}

	int16_t* dyn = malloc(st.st_size);
	uint16_t num;
	ssize_t read_size;
	int index = 0;

	while((read_size = read(fd, &num, sizeof(uint16_t))) > 0){
			if (read_size != (ssize_t)(sizeof(uint16_t))){
				err(5, "Error while reading file: %s", argv[1]);
				exit(-1);
			}
			dyn[index] = num;
			index++;
	}

	for(int i = 0; i < index; i++){
		for(int j = 0; j < index - 1; j++){
			if (dyn[j] > dyn[j+1]){
				int tmp;
				tmp = dyn[j];
				dyn[j] = dyn[j + 1];
				dyn[j + 1] = tmp;
			}
		}
	}

	for(int i = 0; i < index; i++){
		printf("%x\n", dyn[i]);
	}

	free(dyn);
	close(fd);
	exit(0);
}

