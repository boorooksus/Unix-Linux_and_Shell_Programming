#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFSIZE 512
#define PERM 0644

/* 파일 복사 함수 */
/* name1: 복사 파일 이름, name2: 붙여넣기 파일 이름 */
int mycp(const char *name1, const char *name2){
	int infile, outfile;  /* 복사 파일, 붙여넣기 파일 디스크립터 */
	ssize_t nread;  /* 읽은 크기 */
	char buffer[BUFFSIZE];  /* 문자열 저장 */
	
	/* 복사 파일 오픈 */
	if ( (infile = open (name1, O_RDONLY)) == -1){
		return (-1);
	}
	
	/* 붙여넣기 파일 오픈 */
	if ( (outfile = open (name2, O_WRONLY|O_CREAT|O_TRUNC, PERM)) == -1){
		close(infile);
		return (-2);
	}
	
	/* 파일 내용 복사 */
	while( (nread = read (infile, buffer, BUFFSIZE) ) > 0 ){
		if (write(outfile, buffer, nread) < nread){
			close(infile);
			close(outfile);
			return (-3);
		}
	}
	
	/* 파일 닫기 */
	close(infile);
	close(outfile);
	
	if (nread == -1){
		return (-4);
	}
	else{
		return (0);
	}
}

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 3) {
		printf("Wrong input. Usage : %s <file name1> <file name2>\n", argv[0]);
		return 0;
	}
	
	char *filename1 = argv[1];
	char *filename2 = argv[2];
	
	mycp(filename1, filename2);
}
