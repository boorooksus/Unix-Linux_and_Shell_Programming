#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

/* 파일의 permission을 알려주는 함수 */
void whatable(char* filename){
	if (access (filename, R_OK) == -1){
		perror("reading: ");
	} else{
		printf ("%s is readable\n", filename);
	}
	
	if (access (filename, W_OK) == -1){
		perror("writing: ");
	} else{
		printf ("%s is writable\n", filename);
	}
	
	if (access (filename, X_OK) == -1){
		perror("executing: ");
	} else{
		printf ("%s is executable\n", filename);
	}
}

/* 문자열 permission을 정수 형태로 변경하는 함수 */
int lsoct(char perm[]){
	int bi = 0b0;
	int cur = 0b1;
	for(int i = 8; i > -1; i--){
		if(perm[i] != '-'){
			bi |= cur;
		}
		cur <<= 1;
	}
	return bi;
	
}

/* permission 변경 함수 */
int setperm(char* filename, int perm){
	if(chmod(filename, perm) == -1){
		perror("Error: ");
		return 0;
	}
	printf("Permission of %s is changed\n", filename);
	return 0;
}

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 3) {
		printf("Wrong input. Usage : %s <filename> <permission>\n", argv[0]);
		return 0;
	}
	
	int perm = 00;
	/* permission이 문자열로 입력된 경우 */
	if(argv[2][0] == 'r' || argv[2][0] == '-'){
		perm = lsoct(argv[2]);
	} 
	/* permission이 8진수로 입력된 경우 */
	else{
		perm = strtol(argv[2], NULL, 8);
	}
	/* permission이 변경 */
	if(setperm(argv[1], perm) == -1){
		printf("Error is occured\n");
	}
	
}
