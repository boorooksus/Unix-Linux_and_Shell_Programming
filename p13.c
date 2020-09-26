#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define NAMELENGTH 42
#define NROOMS 10
#define BUFFSIZE 512
#define PERM 0644


int infile = -1; // 원본 파일 디스크립터
int outfile = -1; // 복사 파일 디스크립터
char *filename1 = "file1"; // 원본 파일 이름
char *filename2 = "file2"; // 복사 파일 이름
ssize_t nread;
char buffer[BUFFSIZE];

// 파일의 내용을 복사하는 함수
// name1: 복사하는 파일, name2: 붙여넣기 하는 파일, 
// offset: 파일의 복사 내용 위치, flag: lseek함수의 flag
int mycp(const char *name1, const char *name2, off_t offset, int flag){
	// infile open
	if ( (infile = open (name1, O_RDONLY)) == -1){
		return (-1);
	}
	
	// outfile open
	if ( (outfile = open (name2, O_WRONLY|O_CREAT|O_APPEND, PERM)) == -1){
		close(infile);
		return (-2);
	}
	
	// 복사할 영역 탐색
	if (lseek(infile, offset, flag) == -1){
		return (-3);
	}
	
	// 파일 복사
	while( (nread = read (infile, buffer, BUFFSIZE) ) > 0 ){
		// 쓰기 에러가 발생한 경우
		if (write(outfile, buffer, nread) < nread){
			close(infile);
			close(outfile);
			return (-4);
		}
	}
	
	// close the files
	close(infile);
	close(outfile);
	
	if (nread == -1){
		return (-5);
	}
	else{
		return (0);
	}
}

// 파일의 마지막 10줄을 복사하는 함수
int getLines(){
	char buffer[BUFFSIZE]; // 문자열 저장
	int linecnt = 1; // 줄의 총 개수 저장
	int lineOffset[500]; // 각 줄의 시작 위치 저장
	
	/* 첫 번째 줄의 시작 위치는 0 */
	lineOffset[1] = 0;
	
	/* file open */
	if( infile == -1 && (infile = open(filename1, O_RDONLY)) == -1){
		return -1;
	}
	
	// 파일의 줄 수와 각 줄의 위치 탐색
	int cur = 0;
	while( (nread = read(infile, buffer, BUFFSIZE)) > 0){
		 for (int i = 0; i < BUFFSIZE; i++){
			 if (buffer[i] == '\n'){
				 linecnt++;
				 lineOffset[linecnt] = cur * BUFFSIZE + i + 1;
			 }
		 }
		 cur++;
	}
	
	/* 총 줄의 개수가 10줄 이하인 경우 */
	if (linecnt <= 10){
		/* 파일의 전체 내용 복사 */
		mycp(filename1, filename2, 0, 0);
		return 1;
	}
	
	/* 뒤에서 10번째 줄의 시작 위치부터 복사 */
	mycp(filename1, filename2, lineOffset[linecnt - 10], 0);
	return 1;
}

/* 마지막 10문자를 복사하는 함수 */
int getChars(){
	/* file open */
	if( infile == -1 && (infile = open(filename1, O_RDONLY)) == -1){
		return -1;
	}
	
	/* 총 문자의 개수가 10자 이내라면 전체 내용 복사*/
	if(nread = read (infile, buffer, BUFFSIZE) < 11){
		mycp(filename1, filename2, 0, 0);
		return 1;
	}
	
	/* 뒤에서 10번째 문자의 위치 탐색 */
	off_t offset = -11;
	if (lseek(infile, offset, SEEK_END) == -1){
		return -1;
	}
	
	/* 뒤에서 10번째 문자의 위치부터 복사 */
	mycp(filename1, filename2, -11, 2);
	
	/* close the file */
	close(infile);
	return 1;
}

/* 마지막 10단어를 복사하는 함수 */
int getWords(){
	char buffer[BUFFSIZE]; /* 문자열 저장 */
	ssize_t nread; /* read함수가 읽은 바이트 수 */
	int wordcnt = 1; /* 총 단어의 개수 */
	int wordOffset[500]; /* 각 단어의 시작 위치 저장 */
	
	wordOffset[1] = 0; /* 첫 번째 단어의 시작 위치는 0 */

	/* file open */
	if( infile == -1 && (infile = open(filename1, O_RDONLY)) == -1){
		return -1;
	}
	
	/* 단의의 총 개수와 각 단어의 시작 위치 탐색 */
	int cnt = 0;
	while( (nread = read(infile, buffer, BUFFSIZE)) > 0){
		for (int i = 0; i < nread; i++){
			if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\0' || buffer[i] == '\t'){
				 wordcnt++;
				 wordOffset[wordcnt] = cnt * BUFFSIZE + i + 1;
			}
		}
		cnt++;
	}

	/* 파일 내의 총 단어의 개수가 10개 이하이면 전체 내용 복사 */
	if (wordcnt <= 10){
		mycp(filename1, filename2, 0, 0);
		return 1;
	}
	/* 뒤에서부터 10단어 복사 */
	else{
		mycp(filename1, filename2, wordOffset[wordcnt - 10], 0);
		return 1;
	}

}


int main(int argc, char *argv[]){
	printf("1: get last 10 characters of the file\n");
	printf("2: get last 10 words of the file\n");
	printf("3: get last 10 lines of the file\n");
	printf("Input your command number: ");
	
	char x = getchar();
	int y = 0;

	if(x == '1'){
		y = getChars();
		if(y  < 0)
			printf("Error is occured\n");
	}
	else if(x == '2'){
		y = getWords();
		if(y < 0)
			printf("Error is occured\n");
	}
	else{
		y = getLines();
		if(y < 0)
			printf("Error is occured\n");
	}
	
	return 0;
}
