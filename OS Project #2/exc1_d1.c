/*
** !!! program should be compiled with gcc -lrt
*/

#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#include <unistd.h> 		/* fork() ...*/
#include <sys/wait.h>		/* waitpid(),...*/

#define NO_OF_PROCESSES 5
#define END_K 4

sem_t *S1,*S_P1_ends,*S_P2_ends,*S_P3_ends;

void print_func(int id) {
	int k;
	printf("Process %d\n",id);
	for (k=1;k<END_K;k++) {
		printf("\tExecute D%d%d\n",id,k);
	}
}

void P1() {
sem_wait(S1);
	char command[]="ls -l";
	print_func(1);
	//system(command);
	printf("P1 has finished\n");
sem_post(S_P1_ends);
}

void P2() {
sem_wait(S1);
	char command[]="ls -l";
	print_func(2);
	//system(command);
	printf("P2 has finished\n");
sem_post(S_P2_ends);
}

void P3() {
sem_wait(S_P1_ends);
sem_wait(S_P2_ends);
	char command[]="ls -l";
	print_func(3);
	//system(command);
	printf("P3 has finished\n");
sem_post(S_P3_ends);
}

void P4() {
sem_wait(S_P3_ends);
	char command[]="ls -l";
	print_func(4);
	//system(command);
	printf("P4 has finished\n");
sem_post(S_P3_ends);
}

void P5() {
sem_wait(S_P3_ends);
	char command[]="ls -l";
	print_func(5);
	//system(command);
	printf("P5 has finished\n");
sem_post(S_P3_ends);
}

typedef int (*f)();

f process[] = {(void *)&P1,(void *)&P2,(void *)&P3,(void *)&P4,(void *)&P5};      

void child_process_code(int child_id) {
	process[child_id]();
}

int main() { 
    //pid_t child_pid, wpid;
	
    int i;
	pid_t pid;
    system("clear");
	//sem_init(&S1, 1, 2);
	////sem_init(&S_P1_ends, 1, 0);
	//sem_init(&S_P2_ends, 1, 0);
	//sem_init(&S_P3_ends, 1, 0);
	
	S1 = sem_open ("S1", O_CREAT | O_EXCL, 0644, 2); //last argument is the initial value of the semaphore
	S_P1_ends = sem_open ("S_P1_ends", O_CREAT | O_EXCL, 0644, 1);
	S_P2_ends = sem_open ("S_P2_ends", O_CREAT | O_EXCL, 0644, 0);
	S_P3_ends = sem_open ("S_P3_ends", O_CREAT | O_EXCL, 0644, 0);
	
    /* fork child processes */
    for (i = 0; i < NO_OF_PROCESSES; i++){
        pid = fork ();
        if (pid < 0) {
			/* check for error*/
			//sem_unlink ("S1");sem_close(S1);sem_unlink ("S_P1_ends");sem_close(S_P1_ends);
			//sem_unlink ("S_P2_ends");sem_close(S_P2_ends);sem_unlink("S_P3_ends");sem_close(S_P3_ends); 			
            /* unlink prevents the semaphore existing forever */
            /* if a crash occurs during the execution         */
            printf ("Fork error.\n");
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
        printf ("\nParent: All children have exited.\n");
        /* cleanup semaphores */
        sem_unlink ("S1");sem_close(S1);sem_unlink ("S_P1_ends");sem_close(S_P1_ends);
        sem_unlink ("S_P2_ends");sem_close(S_P2_ends);sem_unlink ("S_P3_ends");sem_close(S_P3_ends);		
        /* unlink prevents the semaphore existing forever */  
        /* if a crash occurs during the execution         */
        exit (0);
    }
} 
