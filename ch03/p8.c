#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 2) {
		printf("Wrong input. Usage : %s <filename>\n", argv[0]);
		return 0;
	}
	/* 해당 파일에 writing 권한이 없는 경우 */
	if (access (argv[1], W_OK) == -1){
		perror("Warning: ");
		printf("Do you want to continue?(y/n)\n");
		char x = getchar();
		/* 'n' 입력한 경우 종료 */
		if (x == 'n')
			return 0;
	} 
	/* unlink */
	if(unlink(argv[1]) == -1){
		perror("Error: ");
		return 0;
	}
	
		
}