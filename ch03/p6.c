#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

char *filename = "afile";

void whatable(){
	if (access (filename, R_OK) == -1){
		perror("reading: ");
	} else{
		printf ("%s is readable\n", filename);
	}
	
	if (access (filename, W_OK) == -1){
		perror("writing: ");
	} else{
		printf ("%s is writable\n", filename);
	}
	
	if (access (filename, X_OK) == -1){
		perror("executing: ");
	} else{
		printf ("%s is executable\n", filename);
	}
}

int main(){
	
	whatable();
	
}