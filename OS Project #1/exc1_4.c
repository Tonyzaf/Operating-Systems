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
    printf("The time is %ld",start);
}