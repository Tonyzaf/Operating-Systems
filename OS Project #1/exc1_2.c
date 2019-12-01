#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
    int pid;
    for(int i=0;i<10;i++){
        pid = fork();
        if(pid == 0){
            printf("%d\n",pid);
            break;
        }
    }
}