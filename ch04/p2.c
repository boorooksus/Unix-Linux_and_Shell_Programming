#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

struct stat sb;

int my_double_ls(const char *name){
	struct dirent *d;
	DIR *dp;
	
	/* 디렉토리 개방, 실패여부 점검 */
	if((dp=opendir(name)) == NULL)
		return 1;
	
	/* 디렉토리 살피면서 루프 계속. inode 번호 유효하면 디렉토리항 프린트 */
	while(d = readdir(dp)){
		if(d->d_ino != 0){		
			printf("%s", d->d_name);
			
			/* stat 호출 */
			char fullpath[PATH_MAX];
			sprintf(fullpath, "%s/%s", name, d->d_name);
			
			/* file stat */
			if(stat(fullpath, &sb) == -1){
				fprintf(stderr, "lookout: couldn't stat %s\n", d->d_name);
			}
			/* 디렉토리 파일인 경우 별모양 추가 */
			if(S_ISDIR(sb.st_mode)){
				printf("*\n");
			} else{
				printf("\n");
			}
		}
	}
	
	/* 디렉토리 시작으로 되돌아감 */
	rewinddir(dp);
	
	/* 디렉토리 다시 프린트 */
	while(d = readdir(dp)){
		if(d->d_ino != 0){		
			printf("%s", d->d_name);
			
			/* stat 호출 */
			char fullpath[PATH_MAX];
			sprintf(fullpath, "%s/%s", name, d->d_name);
			/* file stat */
			if(stat(fullpath, &sb) == -1){
				fprintf(stderr, "lookout: couldn't stat %s\n", d->d_name);
			}
			/* 디렉토리 파일인 경우 별모양 추가 */
			if(S_ISDIR(sb.st_mode)){
				printf("*\n");
			} else{
				printf("\n");
			}
		}
	}
	
	closedir(dp);
	return 0;
}

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 2) {
		printf("Wrong input. Usage : %s <directoryname> \n", argv[0]);
		return 0;
	}
	
	my_double_ls(argv[1]);
}