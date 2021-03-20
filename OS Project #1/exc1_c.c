#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
int main(){
pid_t Parent;
pid_t Child;
pid_t Current=getpid();
printf("PID           Parent PID    Child PID\n");
Child = fork();
    if ( Child !=0 ){
        printf("%d          %d             %d\n",Current,Parent,Child);
    }
else return 0;
    for(int i=0;i<9;i++){
        Parent=Current;
        Current=Child;
        Child=fork();
        if ( Child !=0 ){
            printf("%d          %d          %d\n",Current,Parent,Child);
        }
        else return 0;
    }
}
