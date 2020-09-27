#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define SIZE 512

size_t nread;  // 읽은 파일 내용 바이트수
char buf[SIZE];  // 문자열 저장
int filedes = -1; // 파일 디스크립터

/* 파일을 열고 내용을 표준 출력으로 복사하는 함수 */
int fileread(char *filename){
	printf("========== %s ==========\n", filename);
		
	/* file open */
	if ((filedes = open (filename, O_RDONLY)) == -1){
		printf ("Couldn't open %s\n", filename);
		return -1;
	}
	
	/* file read */
	while( (nread = read(filedes, buf, SIZE) ) > 0 ){
		write(1, buf, nread);
	}
	
	return 1;
}

int main(int argc, char *argv[]){
	/* 인수가 없는 경우 */
	if(argc == 1) {
		char filename[SIZE];
		/* 파일 이름을 표준 입력으로 받기 */
		printf("Input filename\n");
		scanf("%s", filename);
		/* 표준 입력으로 받은 파일을 표준 출력으로 복사 */
		fileread(filename);
	}
	
	/* 인수가 있는 경우 */
	else{
		/* 인수로 받은 파일들의 내용을 표준 출력으로 복사 */
		for(int i = 1; i < argc; i++){
			fileread(argv[i]);
		}
	}
	
	exit(0);
}
