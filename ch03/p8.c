#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 2) {
		printf("Wrong input. Usage : %s <filename>\n", argv[0]);
		return 0;
	}
	
	if (access (argv[1], W_OK) == -1){
		perror("Warning: ");
		printf("Do you want to continue?(y/n)\n");
		char x = getchar();
		if (x == 'n')
			return 0;
	} 
	
	if(unlink(argv[1]) == -1){
		perror("Error: ");
		return 0;
	}
	
		
}