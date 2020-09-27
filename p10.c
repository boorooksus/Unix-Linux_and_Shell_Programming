#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define NAMELENGTH 41
#define NROOMS 10

char namebuf[NAMELENGTH];  /* 문자열 저장 */
int infile = -1;  /* 파일 디스크립터 */
off_t offset = 0;  /* 파일 오프셋 */
int minFreeRoom = NROOMS;  /* 빈 방 중 방 번호가 가장 작은 방 */

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
	if(namebuf[1] == ' '){
		/* findfree를 위해 가장 방 번호가 작은 빈방인지 확인 */
		if(roomno < minFreeRoom){
			minFreeRoom = roomno;
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
	minFreeRoom = NROOMS;
	for(int j = 1; j<= NROOMS; j++){
		getoccupier(j);
	}
	return minFreeRoom;
}

/* 방을 비우는 함수 */
/* rn: 비우고자 하는 방 번호 */
int freeroom(int rn){
	
	offset = NAMELENGTH * (rn - 1); /* 방 위치 offset */
	/* 이름을 지우기 위한 문자열 */
	char* blank = "                                        \n";
	
	/* file open */
	if( infile == -1 && (infile = open("residents", O_WRONLY )) == -1){
		return (-1);
	}
	
	/* 방 위치 찾기 */
	if (lseek(infile, offset, SEEK_SET) == -1){
		return (-1);
	}
	
	/* 이름 지우기 */
	if (write( infile, blank, NAMELENGTH - 1) == -1)
		return (-1);
	
	return 1;
}

/* 빈 방인지 조사하고 방을 채우는 함수 */
/* rn: 방 번호, name: 게스트 이름 */
int addguest(int rn, char* origname){
	ssize_t nread;  /* 읽은 바이트 수 */
	/* 방 위치 offset */
	offset = NAMELENGTH * (rn - 1);
	char name[41] = { ' ' };
	name[40] = '\n';
	
	for(int i = 0; i < NAMELENGTH; i++){
		if(origname[i] == '\0')
			break;
		name[i] = origname[i];
	}
	
	/* file open */
	if( infile == -1 && (infile = open("residents", O_RDWR)) == -1){
		return (-1);
	}

	/* 방 위치 찾기*/
	if (lseek(infile, offset, SEEK_SET) == -1){
		return (-1);
	}
	
	/* read file */
	if( (nread = read(infile, namebuf, NAMELENGTH) ) <= 0){
		return (-1);
	}
	
	/* 방이 비어있지 않을 경우 */
	if(namebuf[0] != ' ' && namebuf[1] != ' '){
		return 0;
	}
	
	/* 이름 추가하기 */
	if (lseek(infile, offset, SEEK_SET) == -1){
		return (-1);
	}
	
	if (write( infile, name, NAMELENGTH) == -1)
		return (-1);
	
	return 1;
}
	

int main(){
	/* 명령 입력 받기 */
	printf("1: freeroom\n");
	printf("2: addguest\n");
	printf("Input your command number: ");
	
	char x = getchar();
	
	/* 명령 실행 */
	if(x == '1') {
		printf("========== freeroom ==============\n");
		printf("Input room number to free: ");
		char y[2];
		scanf("%s", y);
		int rn = atoi(y);
		if(freeroom(rn))
			printf("room %2d is free\n", rn);
		else
			printf("Error on freeroom\n");
	} else if (x == '2'){
		printf("========== addguest ==============\n");
		printf("Input room number to add guest: ");
		char y[2];
		scanf("%s", y);
		int rn = atoi(y);
		
		printf("Input guest name: ");
		char name[41];
		scanf("%s", name);
		name[40] = '\n';
		name[41] = '\0';
		
		int res;
		if( res = addguest(rn, name))
			printf("Guest is added to room %2d\n", rn);
		else if(res == 0)
			printf("Room %2d is not empty\n", rn);
		else
			printf("Error on addguest\n");
	} else{
		printf("Wrong input\n");
	}
}
