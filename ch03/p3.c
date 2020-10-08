#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

char ls[10]; /* 변환한 permission 문자열 저장 */

int lsoct(char perm[]){
	/* 비트 연산자를 이용하여 이진수로 변환 후 리턴 */
	int bi = 0b0;
	int cur = 0b1;
	for(int i = 8; i > -1; i--){
		/* '-'가 아닌 경우 1, 맞는 경우 0 */
		if(perm[i] != '-'){
			bi |= cur;
		}
		cur <<= 1;
	}
	return bi;
}

char *octls(char* perm){
	int oc = atoi(perm); /* permission 10진법 정수로 변경 */
	int i = 0; /* ls 인덱스 */
	
	/* oc를 백의 자리수부터 문자열로 변환 */
	int mod = 100; 
	while (oc > 0){
		int cur = oc / mod;
		oc %= mod;
		mod /= 10;
		
		/* ls 각 자리에 해당하는 문자 저장 */
		if(cur >= 4){
			ls[i] = 'r';
			cur -= 4;
		} else{
			ls[i] = '-';
		}
		if(cur >= 2){
			ls[i + 1] = 'w';
			cur -= 2;
		} else{
			ls[i + 1] = '-';
		}
		if(cur >= 1){
			ls[i + 2] = 'x';
		} else{
			ls[i + 2] = '-';
		}
		i += 3;
	}

	return ls;		
}

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 3) {
		printf("Wrong input. Usage : %s <lsoct or octls> <permission>\n", argv[0]);
		return 0;
	}
	/* lsoct를 실행 */
	if(argv[1][0] == 'l'){
		printf("%o\n", lsoct(argv[2]));
	}
	/* octls를 실행 */
	else if(argv[1][0] == 'o'){	
		printf("%s\n", octls((argv[2])));
	}
	/* 잘못된 입력 */
	else{
		printf("Wrong input\n");
	}
	
	return 0;
}