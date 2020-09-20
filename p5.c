#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 512

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 2) {
		printf("Wrong input. Usage : %s <file name>\n", argv[0]);
		exit(1);
	}
	
	char buffer[BUFSIZE];  /* 문자열 저장 */
	int filedes;  /* 파일 디스크립터 */
	int cnt_word = 0;  /* 단어 개수 */
	int cnt_line = 0;  /* 줄의 수 */
	int total = 0;  /* 파일 크기 */
	
	ssize_t nread; /* 읽은 크기 */
	
	char *filename= argv[1]; /* 파일 이름 */
	
	/* 파일 오픈 */
	if (( filedes = open(filename, O_RDONLY)) == -1){
		printf("error in opening %s\n", filename);
		exit(1);
	}
	
	/* 파일 읽기 */
	while( (nread = read(filedes, buffer, BUFSIZE)) > 0){
		 total += nread;
	}
	
	/* 단어, 줄의 개수 탐색 */
	for (int i = 0; i < total; i++){
		if(buffer[i] == '\n' || buffer[i] == ' ' || buffer[i] == '\t'){
			cnt_word++;
		}
		if(buffer[i] == '\n'){
			cnt_line++;
		}
	}
	
	printf("number of words: %ld\n", cnt_word + 1);
	printf("number of lines: %ld\n", cnt_line + 1);
	exit(0);
	
}
