#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define NAMELENGTH 42
#define NROOMS 10
#define BUFFSIZE 512
#define PERM 0644


int filedes = -1; // 파일 디스크립터


int fileopen(char* filename, int flag){
	/* file open */
	if ((filedes = open (filename, flag)) == -1){
		printf ("Couldn't open %s\n", filename);
		return -1;
	}
	return filedes;
}


int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 3) {
		printf("Wrong input. Usage : %s <file name1> <open option>\n", argv[0]);
		return -1;
	}
	
	int res = 0;
	
	/* 일기와 쓰기용으로 파일을 개방 */
	if ( argv[2][0] == 'r' && argv[2][1] == 'w'){
		if ( (res = fileopen (argv[1], 2)) == -1){
			close(filedes);
			return (-1);
		} else{
			printf("The file is opened with write and read mode\n");
		}
	} 
	/* 읽기 전용으로 파일 개방 */
	else if (argv[2][0] == 'r'){
		if ( (res = fileopen (argv[1], 0)) == -1){
			close(filedes);
			return (-1);
		} else{
			printf("The file is opened with read-only\n");
		}		
	} 
	/* 쓰기 전용으로 파일 개방 */
	else if( argv[2][0] == 'w'){
		if ( (res = fileopen (argv[1], 1)) == -1){
			close(filedes);
			return (-1);
		} else{
			printf("The file is opened with write-only\n");
		}
	} 
	/* 자료를 화일의 끝에 덧붙이기 위해 화일을 개방 */
	else if (argv[2][0] == 'a') {
		if ( (res = fileopen (argv[1], O_WRONLY | O_APPEND)) == -1){
			close(filedes);
			return (-1);
		} else{
			printf("The file is opened with append mode\n");
		}
	} else {
		printf("Wrong Input\n");
		
	}

	printf("filedes: %d\n", res);
	
	return res;
}
