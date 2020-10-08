#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

/* 디렉토리 항목 두 번 프린트하는 함수 */
int my_double_ls(const char *name, int skip){
	struct dirent *d;
	DIR *dp;
	
	/* 디렉토리 개방, 실패여부 점검 */
	if((dp=opendir(name)) == NULL)
		return 1;
	
	/* 디렉토리 살피면서 루프 계속. inode 번호 유효하면 디렉토리항 프린트 */
	while(d = readdir(dp)){
		if(d->d_ino != 0){
			if(skip == 1 && d->d_name[0] == '.')
				continue;
			
			printf("%s\n", d->d_name);
		}
	}
	
	/* 디렉토리 시작으로 되돌아감 */
	rewinddir(dp);
	
	/* 디렉토리 다시 프린트 */
	while (d = readdir(dp)){
		if (d->d_ino != 0){
			if(skip == 1 && d->d_name[0] == '.')
				continue;
			
			printf("%s\n", d->d_name);
		}
	}
	
	/* 디렉토리 닫기 */
	closedir(dp);
	return 0;
}

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 3) {
		printf("Wrong input. Usage : %s <directoryname> <skip option>\n", argv[0]);
		return 0;
	}
	
	int skip = atoi(argv[2]);
	
	my_double_ls(argv[1], skip);
}
