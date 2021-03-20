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
    printf("The start time is %lds\n",start);
    int pid;
    int i = 0;
    while(i<1000){
        pid = fork();
        if(pid == 0){
            foo();
            return 0;
        }
    i++;
    }
    for(int y = 0;y<10000;y++){
        waitpid(0,NULL,WUNTRACED);
    }
    time_t end = time(NULL);
    time_t z = end - start;
    //z = z / 1000;
    printf("The end time is %lds\n",end);
    printf("Time to end: %lds\n",z);
}
