#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
void foo(){
    int x = 0;
    x = x + 10;
}
int main(){
    time_t start = time(NULL);
    printf("The time is %ld\n",start);
    int pid;
    int i = 0;
    while(i<100){
        pid = fork();
        if(pid == 0){
            foo();
            printf("%d\n",i);
            break;
        }
    i++;
    }
}