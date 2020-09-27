#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

char *workfile="junk";

int main(){
	int filedes;
	
	/* "junk" 파일이 없는 경우 */
	if ((filedes = open (workfile, O_RDWR)) == -1){
		printf ("Couldn't open %s\n", workfile);
		exit(1);
	}
	
	exit(0);
}