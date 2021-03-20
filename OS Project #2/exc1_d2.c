/*
** !!! program should be compiled with gcc -pthread
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

#define NO_OF_PROCESSES 6
#define END_K 4

sem_t *S1,*S_D1_ends,*S_D2_ends,*S_D3_ends,*S_D4_ends;

void print_func(int id) {
	int k;
	printf("Process %d\n",id);
	for (k=1;k<END_K;k++) {
		printf("\tExecute D%d%d\n",id,k);
	}
}

void D1() {
sem_wait(S1);
	char command[]="ls -l";
	print_func(1);
	//system(command);
	printf("P1 has finished\n");
sem_post(S_D1_ends);
}

void D2() {
sem_wait(S_D1_ends);
	char command[]="ls -l";
	print_func(2);
	//system(command);
	printf("P2 has finished\n");
sem_post(S_D1_ends);
sem_post(S_D2_ends);
}

void D3() {
sem_wait(S_D1_ends);
	char command[]="ls -l";
	print_func(3);
	//system(command);
	printf("P3 has finished\n");
sem_post(S_D1_ends);
sem_post(S_D3_ends);
}

void D4() {
sem_wait(S_D2_ends);
	char command[]="ls -l";
	print_func(4);
	//system(command);
	printf("P4 has finished\n");
sem_post(S_D2_ends);
sem_post(S_D4_ends);
}

void D5() {
sem_wait(S_D2_ends);
sem_wait(S_D3_ends);
	char command[]="ls -l";
	print_func(5);
	//system(command);
	printf("P5 has finished\n");
sem_post(S_D2_ends);
sem_post(S_D3_ends);
}

void D6() {
sem_wait(S_D3_ends);
sem_wait(S_D4_ends);
	char command[]="ls -l";
	print_func(6);
	//system(command);
	printf("P6 has finished\n");
sem_post(S_D3_ends);
sem_post(S_D4_ends);
}

typedef int (*f)();

f process[] = {(void *)&D1,(void *)&D2,(void *)&D3,(void *)&D4,(void *)&D5,(void *)&D6};      

void child_process_code(int child_id) {
	process[child_id]();
}

int main() { 
    int i;
	pid_t pid;
    system("clear");

	S1 = sem_open ("S1", O_CREAT | O_EXCL, 0644, 1); //last argument is the initial value of the semaphore
	S_D1_ends = sem_open ("S_D1_ends", O_CREAT | O_EXCL, 0644, 0);
	S_D2_ends = sem_open ("S_D2_ends", O_CREAT | O_EXCL, 0644, 0);
	S_D3_ends = sem_open ("S_D3_ends", O_CREAT | O_EXCL, 0644, 0);
	S_D4_ends = sem_open ("S_D4_ends", O_CREAT | O_EXCL, 0644, 0);
	
    /* fork child processes */
    for (i = 0; i < NO_OF_PROCESSES; i++){
        pid = fork ();
        if (pid < 0) {
			/* check for error*/
			//sem_unlink ("S1");sem_close(S1);sem_unlink ("S2");sem_close(S2);
			//sem_unlink ("S_D2_ends");sem_close(S_D2_ends);sem_unlink("S_D3_ends");sem_close(S_D3_ends); 			
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
        sem_unlink ("S1");sem_close(S1);sem_unlink ("S_D1_ends");sem_close(S_D1_ends);
        sem_unlink ("S_D2_ends");sem_close(S_D2_ends);sem_unlink ("S_D3_ends");sem_close(S_D3_ends);
		sem_unlink ("S_D4_ends");sem_close(S_D4_ends);		
        /* unlink prevents the semaphore existing forever */
        /* if a crash occurs during the execution         */
        exit (0);
    }
} 
