#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char** argv){
	if (argc != 3) {
		errx(1, "Please enter 2 arguments!");
		exit(-1);
	}

	if (strcmp("--max", argv[1]) != 0 && strcmp("--min", argv[1]) != 0 && strcmp("--print",argv[1]) != 0){
		errx(2, "Wrong arguments!");
		exit(-1);
	}
	
	int fd = open(argv[2], O_RDONLY);
	
	if (fd == -1) {
		err(3, "Error opening file: %s", argv[2]);
		exit(-1);
	}

	struct stat st;

	if(stat(argv[2],&st) == -1){
		err(4, "Error using stat with file: %s", argv[2]);
		exit(-1);
	}
	
	if(st.st_size == 0){
		err(5, "File is empty!");
		exit(0);
	}
	
	int16_t* dyn = malloc(st.st_size);   // Look man 2 stat and man malloc
	uint16_t num;
	ssize_t read_size;
	int index = 0;

	while((read_size = read(fd, &num, sizeof(uint16_t))) > 0){
		if(read_size != (ssize_t)(sizeof(uint16_t))){
			errx(6, "Error reading: %s",argv[2]);
		}
		dyn[index] = num;
		index++;
	}
	
	uint16_t max = 0;
	uint16_t min = 65534;
	
	for(int i = 0; i < index; i++){
		if (strcmp("--print", argv[1]) == 0){
			printf("%x\n", dyn[i]);
		}
		else {
			if (dyn[i] > max) max = dyn[i];
			if (dyn[i] < min) min = dyn[i];
		}
	}

	if(strcmp("--max",argv[1]) == 0){
		printf("%x\n", max);
	}

	if(strcmp("--min",argv[1]) == 0){
		printf("%x\n", min);
	}

	free(dyn);
	close(fd);
	exit(0);
}


	
	
	





	
