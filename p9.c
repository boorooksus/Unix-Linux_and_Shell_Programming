#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define NAMELENGTH 42
#define NROOMS 10

char namebuf[NAMELENGTH];  /* 문자열 저장 */
int infile = -1;  /* 파일 디스크립터 */
off_t offset = 0;  /* 파일 오프셋 */
int freeroom = NROOMS;  /* 빈 방 중 방 번호가 가장 작은 방 */

/* 방 조사 함수 */
/* roomno: 방 번호 */
/* 방이 빈 경우 empty, 아닌 경우 거주자 이름 리턴 */
char *getoccupier(int roomno){
	ssize_t nread;
	
	/* file open */
	if( infile == -1 && (infile = open("residents", O_RDONLY)) == -1){
		return (NULL);
	}

	/* 방 위치 찾은 후, 투숙객 이름 읽기 */
	if (lseek(infile, offset, SEEK_SET) == -1){
		return (NULL);
	}
	
	/* read file */
	if( (nread = read(infile, namebuf, NAMELENGTH) ) <= 0){
		return (NULL);
	}
	
	/* 방이 비어있을 경우 */
	if(namebuf[0] == ' ' && namebuf[1] == ' '){
		/* findfree를 위해 가장 방 번호가 작은 빈방인지 확인 */
		if(roomno < freeroom){
			freeroom = roomno;
		}
		offset += NAMELENGTH;
		char *empty = "Empty";
		return(empty);
	}
	
	/* 거주자가 있을 경우 */
	
	offset += NAMELENGTH;
	
	/* 개행문자를 마지막에 넣어 문자열 생성 */
	namebuf[NAMELENGTH - 1] = '\0';
	return (namebuf);
}

/* 빈 방 탐색 함수 */
/* 빈 방 중 방 번호가 가장 작은 방 리턴 */
int findfree(){
	offset = 0;
	for(int j = 1; j<= NROOMS; j++){
		getoccupier(j);
	}
	return freeroom;
}

int main(){
	int j;
	char *getoccupier (int), *p;
	
	printf("========== getoccupier ==============\n");
	offset = 0;
	for(j = 1; j<= NROOMS; j++){
		if(p = getoccupier(j)){
			printf("room %2d, %s\n", j, p);
		} else{
			printf("Error on room &d\n", j);
		}
	}
	
	printf("============= findfree ===============\n");
	printf("free roome is %2d\n", findfree());
}