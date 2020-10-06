#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

char ls[10];

int lsoct(char perm[]){
	int res = 0;
	
	for(int i = 0; perm[i] != '\0'; i += 3){
		int temp = 0;
		for(int j = 2; j >= 0; j--){
			if (perm[i + 2 - j] != '-'){
				int sqrt = 1;
				for(int k = 0; k < j; k++){
					sqrt *= 2;
				}
				temp = temp + sqrt;
				
			}
		}
		res = res * 10 + temp;
	}
	
	return res;
}

char *octls(char* perm){
	int oc = atoi(perm);
	
	int u = oc / 100;
	oc %= 100;
	if(u >= 4){
		ls[0] = 'r';
		u -= 4;
	} else{
		ls[0] = '-';
	}
	if(u >= 2){
		ls[1] = 'w';
		u -= 2;
	} else{
		ls[1] = '-';
	}
	if(u >= 1){
		ls[2] = 'x';
	} else{
		ls[2] = '-';
	}
	
	int g = oc / 10;
	oc %= 10;
	if(g >= 4){
		ls[3] = 'r';
		g -= 4;
	} else{
		ls[3] = '-';
	}
	if(g >= 2){
		ls[4] = 'w';
		g -= 2;
	} else{
		ls[4] = '-';
	}
	if(g >= 1){
		ls[5] = 'x';
	} else{
		ls[5] = '-';
	}
	
	int o = oc;
	if(o >= 4){
		ls[6] = 'r';
		o -= 4;
	} else{
		ls[6] = '-';
	}
	if(o >= 2){
		ls[7] = 'w';
		o -= 2;
	} else{
		ls[7] = '-';
	}
	if(o >= 1){
		ls[8] = 'x';
	} else{
		ls[8] = '-';
	}

	return ls;
		
}

int main(int argc, char *argv[]){
	/* 명령이 잘못된 경우 */
	if(argc != 3) {
		printf("Wrong input. Usage : %s <lsoct or octls> <permission>\n", argv[0]);
		return 0;
	}
	
	if(argv[1][0] == 'l'){
		printf("%d\n", lsoct(argv[2]));
	}
	else if(argv[1][0] == 'o'){	
		printf("%s\n", octls((argv[2])));
	}
	else{
		printf("Wrong input\n");
	}
	
	return 0;
}