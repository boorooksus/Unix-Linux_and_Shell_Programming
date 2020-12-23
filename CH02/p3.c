#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#define PERMS 0644

char *filename= "newfile";

int main(){
	int filedes; /* 파일 디스크립터 */
	
	/* 파일 생성 */
	if ((filedes = creat("./newfile", PERMS)) == -1){
		/* 에러가 있는 경우 */
		printf("Couldn't create %s\n", filename);
		exit(1);
	}
	else{
		printf("%s is created\n", filename);
	}
	
	/* 파일 open */
	if ((filedes = open (filename, O_RDWR, PERMS)) == -1){
		/* 에러가 있는 경우 */
		printf("Couldn't open %s\n", filename);
		exit(1);
	}
	else {
		printf("%s is opened\n", filename);
	}
	
	exit(0);
}
