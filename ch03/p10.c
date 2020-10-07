#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define MFILE 10
#define BUFSIZE 512

void cmp(const char *, time_t, int j);
void slowwatch(int argc, char* argv[]);
struct stat sb;
time_t last_time[MFILE + 1];
void fileread(const char *);

int main(int argc, char *argv[]){

	
	if(argc <2){
		fprintf(stderr, "usage: lookout filename ...\n");
		exit(1);
	}
	
	if(argc > MFILE){
		fprintf(stderr, "lookout: too many filenames\n");
		exit(1);
	}
	
	slowwatch(argc, argv);
}

void slowwatch(int argc, char* argv[]){
	int j;
	
	/* 초기화 */
	for(j = 1; j < argc; j++){
		/*
		printf("argc: %d\n", argc);
		printf("filename: %s\n", argv[j]);
		*/
		if(stat(argv[j], &sb) == -1){
			fprintf(stderr, "lookout: couldn't stat %s\n", argv[j]);
			//exit(1);
			//continue;
		}
		last_time[j] = sb.st_mtime;
	}
	
	/* 파일 변경될 때가지 루프 */
	for(;;){
		for(j = 1; j < argc; j++)
			cmp(argv[j], last_time[j], j);
		sleep(3);
	}
	
}

void cmp(const char *name, time_t last, int j) {
	/* 변경시간 검사*/
	if (stat(name, &sb) == -1 || sb.st_mtime != last){
		fprintf(stderr, "lookout: %s changed\n", name);
		last_time[j] = sb.st_mtime;
		fileread(name);
		//exit(0);
	}
}

void fileread(const char *filename){
	char buffer[BUFSIZE];  /* 문자열 저장 */
	int filedes;  /* 파일 디스크립터 */
	
	ssize_t nread; /* 읽은 크기 */
	
	/* 파일 오픈 */
	if (( filedes = open(filename, O_RDONLY)) == -1){
		printf("error in opening %s\n", filename);
		exit(1);
	}
	/* 파일 읽기 */
	while( (nread = read(filedes, buffer, BUFSIZE - 1)) > 0){ 
		buffer[nread] = '\0';
		//for(int i = 0; i < nread; i++)
			//printf("%c", buffer[i]);
		fprintf(stderr, "contents:\n%s", buffer);
	}
	printf("\n");
	/* 파일 클로즈 */
	if (close(filedes) == -1){
		printf("error in closing %s\n", filename);
		exit(1);
	}
}
	
