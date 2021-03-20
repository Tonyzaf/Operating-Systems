/*
** !!! program should be compiled with gcc -lrt
*/

#include <stdio.h>          /* printf(),...             */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#include <unistd.h> 		/* fork() ...*/
#include <sys/wait.h>		/* waitpid(),...*/


#define NO_OF_PROCESSES 20
#define BUFFER_SIZE 40

sem_t *S2;

void child_process_code(int select_op) {
    if (select_op%9==0) { //Writing process
		sem_wait(S2);		
		FILE *fp=fopen("output.txt","w");
		fprintf(fp, "%d : I am process %d and I am doing output to the shared_file\n", select_op,getpid());
		fclose(fp);
		printf("%d process wrote to the shared file\n",select_op);
		sem_post(S2);
	} else {//Reading process
		FILE *fp;
		char buf[BUFFER_SIZE];
		fp = fopen("output.txt", "r");
		char *s = fgets(buf,BUFFER_SIZE,fp);
		fclose(fp);
		if (s!=NULL) printf("%d process read :'%s'\n",select_op,buf);
		else printf("%d process found the shared file empty\n", select_op);
	}
}

int main() {
	pid_t pid;
	pid_t wpid;
	int i;
	int status;
	
    S2 = sem_open("S2", O_CREAT | O_EXCL, 0644, 1); //last argument is the initial value of the semaphore

	/* fork child processes */
    for (i = 0; i < NO_OF_PROCESSES; i++) {
        pid = fork ();
        if (pid < 0) {
			/* check for error*/
            perror("Fork error.\n");
        }
        else if (pid == 0) {
            break;                  /* child processes */
		}
    }
	if (pid==0){
		/******************************************************/
		/******************   CHILD PROCESS   *****************/
		/******************************************************/
		child_process_code(i);
        exit (0);
    } else {
		/******************************************************/
		/******************   PARENT PROCESS   ****************/
		/******************************************************/
        /* wait for all children to exit */
        while (pid = waitpid (-1, NULL, 0)){
            if (errno == ECHILD)
                break;
        }
		printf("\nParent: All children have exited.\n");
        /* cleanup semaphore*/
        sem_unlink ("S2");sem_close(S2);
		exit(0);
    }
}
