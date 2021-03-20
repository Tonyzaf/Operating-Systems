#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
int main(){
    int pid;
    for(int i=0;i<10;i++){
        pid = fork();
        if(pid == 0){
            printf("Child %d Proccess %d\n",i,pid);
            return 0;
        }
    }
    printf("Parent Proccess = %d\n", pid);
}
