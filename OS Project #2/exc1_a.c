#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 

int main() {
	int pid1;
	int pid2;
  
	pid1 = fork();
	if (pid1 < 0)
		printf("Could not create any child\n");
	else {
		pid2 = fork();
      		if (pid2 < 0)
			printf("Could not create any child\n");
		else
			if ((pid1 < 0) && (pid2 < 0)) printf("kill(pid1,9)\n");
    	}
   	//printf("hello!1=%d,2=%d\n",pid1,pid2);
	if ((pid1>0)&&(pid2>0)) printf("Starting initial process id=%d\n",getpid());
   	else printf("My id is %d and my parent's id is %d\n",getpid(),getppid());
   	//sleep(20);
   	return (0);
}
