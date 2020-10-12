#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
 #include <sys/stat.h>
 #include <stdlib.h>
 
 struct stat sb;

/* 문자열 permission을 정수 형태로 변경하는 함수 */
int lsoct(char perm[]){
	int bi = 0b0;
	int cur = 0b1;
	/* 문자열 맨 앞에 파일 종류 정보가 있는 경우 */
	if(perm[9] != '\0'){
		for(int i = 9; i > 0; i--){
		if(perm[i] != '-'){
			bi |= cur;
		}
		cur <<= 1;
		}
	}
	/* 문자열 맨 앞에 파일 종류 정보가 없는 경우 */
	else{
		for(int i = 8; i > -1; i--){
			if(perm[i] != '-'){
				bi |= cur;
			}
		cur <<= 1;
		}
	}
	
	return bi;
}

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 3) {
		printf("Wrong input. Usage : %s <mode> <filename>\n", argv[0]);
		return 1;
	}
	/* Absolute mode인 경우 */
	int mode = 0b0;
	/* 문자열 permission이 입력된 경우 */
	if(argv[1][0] == '-' || argv[1][0] =='r'){
		mode = lsoct(argv[1]);
		if (chmod(argv[2], mode)){
			return 2;
		}
		return 0;
	} 
	/* 8진수 permission이 입력된 경우 */
	else if(argv[1][0] >= '0' && argv[1][0] < '8'){
		mode = strtol(argv[1], NULL, 8);
		if (chmod(argv[2], mode)){
			return 2;
		}
		return 0;
	}
	
	/* Symbolic mode인 경우 */
	
	/* file stat */
	if(stat(argv[2], &sb) == -1){
		perror("Error: ");
		return 3;
	}
	/* cur: 현재 파일의 권한 */
	int cur = sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
	int target = 0b0; /* 권한 변경할 영역(user, group, other 관점) */
	int permission = 0b0; /* 권한 변경할 영역(read, write, exec 관점) */
	int op = 0; /* 연산자 종류 저장(1: '+', 2: '-', 3: '=') */
	int res = 0b0; /* Absolute mode */
	int i = 0; /* 현재 while문의 인덱스 */
	
	/* 두 번째 인자 문자열의 각 문자 탐색 */
	/* 두 번째 인자: [who][+|-+=][mode...] 구조 */
	while(1){
		/* who 알아내기 */
		if(argv[1][i] == 'u')
			target += 0b111000000;
		else if(argv[1][i] == 'g')
			target += 0b000111000;
		else if(argv[1][i] == 'o')
			target += 0b000000111;
		else if(argv[1][i] == 'a')
			target += 0b111111111;
		/* 연산자 알아내기 */
		else if(argv[1][i] == '+')
			op = 1;
		else if(argv[1][i] == '-' && i > 0 && (argv[1][i-1] == 'u' || argv[1][i-1] == 'g' || argv[1][i-1] == '0'))
			op = 2;
		else if(argv[1][i] == '=')
			op = 3;
		/* mode 알아내기 */
		else if(argv[1][i] == 'r')
			permission += 0b100100100;
		else if(argv[1][i] == 'w')
			permission += 0b010010010;
		else if(argv[1][i] == 'x')
			permission += 0b001001001;
		else if(argv[1][i] == '-' && op == 3)
			permission += 0b0;
		/* 다른 옵션이 있거나 문자열이 끝난 경우 계산 후 권한 변경*/
		else if(argv[1][i] == ',' || argv[1][i] == '\0'){
			res = target & permission;
			/* +연산자인 경우 */
			if(op == 1){
				res |= cur;
			} 
			/* -연산자인 경우 */
			else if(op == 2){
				res = cur - (res & cur);
			}
			/* =연산자인 경우 */
			else if(op == 3){
				res = res | (cur - (target & cur));	
			}
			/* 권한 변경 */
			if (chmod(argv[2], res)){
				return 2;
			}
			/* 변수 초기화 */
			target = 0b0;
			permission = 0b0;
			op = 0;
		} 
		/* 잘못된 입력이 있는 경우 */
		else{
			printf("Error: Wrong input\n");
			return 4;
		}
		/* 문자열이 끝난 경우  루프 탈출 */
		if(argv[1][i] == '\0')
			break;
		/* while 루프 인덱스 증가 */
		i++;
	}
	/* success */
	return 0;
}