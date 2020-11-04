#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];
int numtokens = 0;
pid_t pid = -1; /* pid를 글로벌 변수로 변경 */

void fatal(char *str){
	perror(str);
	exit(1);
}

int makelist(char *s, const char *delimiters, char** list, int MAX_LIST){	
  int i = 0;
  int numtokens = 0;
  char *snew = NULL;
  
  if( (s==NULL) || (delimiters==NULL) ) return -1;

  snew = s + strspn(s, delimiters);	/* Skip delimiters */
  if( (list[numtokens]=strtok(snew, delimiters)) == NULL )
    return numtokens;
	
  numtokens = 1;
  
  while(1){
     if( (list[numtokens]=strtok(NULL, delimiters)) == NULL)
	break;
     if(numtokens == (MAX_LIST-1)) return -1;
     numtokens++;
  }
  return numtokens;
}

/* 시그널 핸들러 */
void sighandler(int signal){
	/* 자식 프로세스인 경우에만 exit */
	if(pid == 0){
		exit(0);
	}
}

/* SIGCHLD 시그널 핸들러 */
void sigchldhandler(int signal){
	wait(NULL);
}

int main(int argc, char**argv){
  int i=0;
  
  /* SIGINT, SIGQUIT에 쉘 종료되지 않게 설정 */
  signal(SIGINT, sighandler);
  signal(SIGQUIT, sighandler);
  /* 자식 프로세스 종료 시 wait() 실행되도록 설정 */
  signal(SIGCHLD, sigchldhandler);
  
  while (1) {
  	fputs(prompt, stdout);
	fgets(cmdline, BUFSIZ, stdin);
	cmdline[strlen(cmdline) -1] = '\0';
	
	/* 명령을 받은 직후 makelist 실행 */
	/* numtokens: command line 단어 개수 */
	numtokens = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
	
	/* 명령 내용이 없는 경우 */
	if(numtokens == 0){
		continue;
	}
	/* cd 명령어가 입력된 경우 */
	else if(strcmp(cmdvector[0], "cd") == 0 ){
		if(chdir(cmdvector[1]) == -1){
			fatal("main()");
		}
	} 
	/* exit 명령어가 입력된 경우 */
	else if(strcmp(cmdvector[0], "exit") == 0 ){
		/* 인자 존재 여부에 따라 exit status 설정 */
		if(numtokens == 1)
			exit(0);
		else
			exit(atoi(cmdvector[1])); /* exit status 와 함께 종료 */
	} 
	/* 백그라운드로 실행 */
	else if(cmdvector[numtokens - 1][strlen(cmdvector[numtokens - 1]) - 1] == '&'){	
		/* 명령 수행을 위해 '&'기호 제거 */
		if(strlen(cmdvector[numtokens - 1]) == 1)
			cmdvector[numtokens - 1] = NULL;
		else
			cmdvector[numtokens - 1][strlen(cmdvector[numtokens - 1]) - 1] = '\0';
		
		/* child process 생성 후 명령 실행 */
		/* (parent process는 wait()을 호출하지 않는다) */
		switch(pid=fork()){
			case 0:
				execvp(cmdvector[0], cmdvector);
				fatal("main()");
			case -1:
				fatal("main()");
		}
	} 
	/* 위에 해당사항이 없을 때 */
	else{
		/* child process 생성 후 명령 실행 */
		/* (parent process는 wait()으로 child process 수행을 기다린다.) */
		switch(pid=fork()){
		case 0:
			execvp(cmdvector[0], cmdvector);
			fatal("main()");
		case -1:
			fatal("main()");
		default:
			wait(NULL);
		}
	}
  }
  return 0;
}
