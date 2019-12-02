#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
void foo(){
    int x = 0;
    x = x + 10;
}
int main(){
    time_t start = time(NULL);
    printf("The time is %ld\n",start);
    int pid;
    int i = 0;
    while(i<1000){
        pid = fork();
        if(pid == 0){
            foo();
            break;
        }
    i++;
    }
    for(int y = 0;y<10000;y++){
        waitpid(0,NULL,WUNTRACED);
    }
    time_t end = time(NULL);
    double z = (double)end - (double)start;
    z = z / 1000;
    printf("Time to end: %f\n",z);
}