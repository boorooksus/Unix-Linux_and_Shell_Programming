#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];
int numtokens = 0;
pid_t pid = -1; /* pid를 글로벌 변수로 변경 */
char* cmdpipe;
char  cmdline2[BUFSIZ];
char* cmdvector2[MAX_CMD_ARG];
char* cmdvector3[MAX_CMD_ARG];
char* pch;
int numtokens2 = 0;
int p[2], status;

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

/* redirection in 함수 */
int redirection_in(char **cmdvector){	
	int idx = 0, fd = 0;
	/* cmdvector에서 "<" 위치 찾기 */
	while(cmdvector[idx] != NULL){
		if(strcmp(cmdvector[idx], "<") == 0)
			break;
		idx++;
	}
	
	if(idx == 0 || !cmdvector[idx + 1])
		return -1;
	/* redirection */
	if((fd = open(cmdvector[idx+1], O_RDONLY)) == -1){
		fatal("(redirection_in)");
		return -1;
	}
	dup2(fd, 0);
	close(fd);
	/* cmdvector에서 '<' 이후 내용 정리 */
	while(cmdvector[idx] != NULL){
		cmdvector[idx] = cmdvector[idx + 2];
		idx++;
	}
	cmdvector[idx] = NULL;

	return 0;
}

/* redirection out 함수 */
int redirection_out(char **cmdvector){
	int idx = 0;
	int fd = 0;
	/* cmdvector에서 ">" 위치 찾기 */
	while(cmdvector[idx] != NULL){
		if(strcmp(cmdvector[idx], ">") == 0){
			break;
		}
		idx++;
	}
	
	if(idx == 0 || !cmdvector[idx + 1]){
		
		return -1;
	}
	/* redirection */
	if((fd = open(cmdvector[idx+1], O_RDWR|O_CREAT,0644)) == -1){
		fatal("(redirection_out)");
		return -1;
	}
	dup2(fd, 1);
	close(fd);
	/* cmdvector에서 '>' 이후 내용 정리 */
	while(cmdvector[idx] != NULL){
		cmdvector[idx] = cmdvector[idx + 2];
		idx++;
	}
	cmdvector[idx] = NULL;
	
	return 0;
}

/* 파이프 수행 함수 */
int mypipe(char **cmdvector){
	int pipe_cnt = 0;
	int pipe_idx[4];
	/* 첫 번째 파이프 위치 확인 */
	for(int i = 0; cmdvector[i] != NULL; i++){
		if(strcmp(cmdvector[i], "|") == 0){
			pipe_cnt++;
			pipe_idx[pipe_cnt] = i;
		}
	}
	
	if(pipe_cnt == 0){
		return -1;
	}
	/* cmdvector의 첫 번째 파이프 위치 값을 NULL로 변경 */
	cmdvector[pipe_idx[1]] = NULL;
	
	/* cmdvector2에 첫 번째 파이프 이후 내용을 옮긴다 */
	int i = pipe_idx[1] + 1;
	int cur = 0;
	for(i = i; cmdvector[i] != NULL && strcmp(cmdvector[i], "|") != 0; i++){
		cmdvector2[cur] = cmdvector[i];
		cur++;
	}
	cmdvector2[cur] == NULL;
	
	if(pipe_cnt == 2){
		/* cmdvector2의 파이프 이후 내용 cmdvector3에 옮긴다 */
		i = pipe_idx[2] + 1;
		cur = 0;
		for(; cmdvector[i] != NULL; i++){
			cmdvector3[cur] = cmdvector[i];
			cur++;
		}
		cmdvector3[cur] == NULL;
	}

	/* parent process */
	switch(pid=fork()){
	case 0:
		break;
	case -1:
		fatal("(pipe first fork) ");
	default:
		wait(&status);
		if(pipe_cnt < 2){
			return(status);
		}
	}
	if(pipe(p) == -1){
		fatal("(pipe call) ");
	}	
	/* child process */
	switch(pid=fork()){
	case 0:
		/* redirection 있는지 확인 */
		if(strchr(cmdline2, '<')){
			redirection_in(cmdvector);
		}
		dup2 (p[1], 1);
		close(p[0]);
		close(p[1]);
		execvp(cmdvector[0], cmdvector);
		fatal("(pipe child) ");
	case -1:
		fatal("(pipe second fork) ");
	default:
		if(pipe_cnt == 1 && strchr(cmdline2, '>')){
			redirection_out(cmdvector);
		}
		dup2 (p[0], 0);
		close(p[0]);
		close(p[1]);
		execvp(cmdvector2[0], cmdvector2);
		fatal("(pipe parent) ");
	}	

	/* 파이프가 두 개일 때 */
	if(pipe_cnt == 2){
		if(pipe(p) == -1){
			fatal("(pipe call) ");
		}
	
		switch(pid=fork()){
		case 0:
			dup2 (p[1], 1);
			close(p[0]);
			close(p[1]);
			fatal("(pipe child) ");
		case -1:
			fatal("(pipe second fork) ");
		default:
			/* redirection 있는지 확인 */
			if(strchr(cmdline2, '>')){
				redirection_out(cmdvector);
			}
			dup2 (p[0], 0);
			close(p[0]);
			close(p[1]);
			execvp(cmdvector3[0], cmdvector3);
			fatal("(pipe parent) ");
		}	
	}
	return 0;
}

int main(int argc, char**argv){
  int i=0;
  
  /* SIGINT, SIGQUIT에 쉘 종료되지 않게 설정 */
  signal(SIGINT, sighandler);
  signal(SIGQUIT, sighandler);
  
  while (1) {
  	fputs(prompt, stdout);
	fgets(cmdline, BUFSIZ, stdin);
	cmdline[strlen(cmdline) -1] = '\0';
	
	/* cmdline 복사 */
	strncpy(cmdline2, cmdline, sizeof(cmdline2));
	
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
	/* 파이프 있을 때 */
	else if(strchr(cmdline2, '|')){
		mypipe(cmdvector);
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
				/* redirection 있는지 확인 */
				if(strchr(cmdline2, '<')){
					redirection_in(cmdvector);
				}
				if(strchr(cmdline2, '>')){
					redirection_out(cmdvector);
				}
				execvp(cmdvector[0], cmdvector);
				fatal("main()");
			case -1:
				fatal("main()");
			default:
				/* 자식 프로세스 종료 시 wait() 실행되도록 설정 */
				signal(SIGCHLD, sigchldhandler);
		}
	} 
	/* 위에 해당사항이 없을 때 */
	else{
		/* child process 생성 후 명령 실행 */
		/* (parent process는 wait()으로 child process 수행을 기다린다.) */
		switch(pid=fork()){
		case 0:	
			/* redirection 있는지 확인 */
			if(strchr(cmdline2, '<')){
				redirection_in(cmdvector);
			}
			if(strchr(cmdline2, '>')){
				redirection_out(cmdvector);
			}
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
