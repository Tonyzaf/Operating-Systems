#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
    printf("PID     Parent PID      Child PID\n");
    int pid;
    for(int i=0;i<10;i++){
        pid = fork();
        if(pid == 0){
            printf("%d          %d          %d\n",getppid(),getpid(),pid);
            break;
        }
    }
}