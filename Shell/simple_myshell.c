#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_CMD_ARG 10
#define BUFSIZ 256

const char *prompt = "myshell> ";
char* cmdvector[MAX_CMD_ARG];
char  cmdline[BUFSIZ];
//static char directory[1024];
int numtokens = 0;

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

int cd(char* dir){
	if(chdir(dir) == -1){
		fatal("main()");
	}
	return 0;
}

int main(int argc, char**argv){
  int i=0;
  pid_t pid;
  while (1) {
  	fputs(prompt, stdout);
	fgets(cmdline, BUFSIZ, stdin);
	cmdline[strlen(cmdline) -1] = '\0';
	
	numtokens = makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
	
	if(strcmp(cmdvector[0], "cd") == 0 ){
		cd(cmdvector[1]);
		continue;
	} else if(strcmp(cmdvector[0], "exit") == 0 ){
		exit(0);
	} else if(strcmp(cmdvector[numtokens - 1], "&") == 0){
		//fputs("in Background!! \n", stdout);
		cmdvector[numtokens - 1] = NULL;
		
		switch(pid=fork()){
			case 0:
				execvp(cmdvector[0], cmdvector);
				fatal("main()");
			case -1:
				fatal("main()");
		}
	} else{
		switch(pid=fork()){
		case 0:
			//fputs("This is child \n", stdout);
			//makelist(cmdline, " \t", cmdvector, MAX_CMD_ARG);
			//fputs("after makelist \n", stdout);
			//fputs("cmdvector[0]: ", stdout);
			//fputs(cmdvector[0], stdout);
			//fputs("\n", stdout);
			//fputs("cmdvector[1]: ", stdout);
			//fputs(cmdvector[1], stdout);
			//fputs("\n", stdout);
			execvp(cmdvector[0], cmdvector);
			fatal("main()");
			//fputs("after execvp \n", stdout);
			//exit(directory);
		case -1:
			fatal("main()");
		default:
			//fputs("pid: ", stdout);
			//fputs(pid, stdout);
			//fputs("hi \n", stdout);
			wait(NULL);
		}
	}
  }
  return 0;
}
