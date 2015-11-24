#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

/*creating pipe between graphGlen and multi-thread*/
	int fds_12[2]; //data written to the file descriptor read [0] can be read back from write [1]
	int pid_rgen;
	int i,j;
	char* buffer[1];
	char buf_1[1];
	char buf_2[2];
	
	pipe (fds_12);  

	pid_rgen = fork();  
	
	if (pid_rgen > 0) {
	 	close (fds_12[0]);
		dup2 (fds_12[1],STDOUT_FILENO); 
	
		for (i = 5; i<=50; i=i+5){
			for(j=0;j<10;j++){


				if(i=5){
					sprintf(buf_1,"%d",i);
					buffer[0] = buf_1;
					execv ("/home/wdietl/graphGen/graphGen",buffer);  //possible error place
				}
				else{
					sprintf(buf_2,"%d",i);
					buffer[0] = buf_2;
					execv ("/home/wdietl/graphGen/graphGen",buffer); 
				}
			}			
			
		}
		
	}
	else{
	 	close (fds_12[1]);
		dup2 (fds_12[2],STDIN_FILENO); 		
		
		execlp ("./multi_thread","multi_thread",NULL);
		
	}

	
	return 1;
}



