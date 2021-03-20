/*
** !!! program should be compiled with gcc -pthread
*/

#include <stdio.h>          /* printf(),...             */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#include <unistd.h> 		/* fork() ...*/
#include <sys/wait.h>		/* waitpid(),...*/

sem_t *Sem1;

#define NO_OF_PROCESSES 20
#define UPPER 1000
#define LOWER 1

/* Sudhanshu Patel sudhanshuptl13@gmail.com 
**
** Min Heap implementation in c
** Array Implementation of MinHeap data Structure
**
*/
struct Heap{
    int *arr;
    int count;
    int capacity;
    int heap_type; // for min heap , 1 for max heap
};
typedef struct Heap Heap;

void CreateHeap(int capacity,int heap_type);
void insert(Heap *h, int key);
void print(Heap *h);
void heapify_bottom_top(Heap *h,int index);
void heapify_top_bottom(Heap *h, int parent_node);
int PopMin(Heap *h);

static Heap *HeapStr;

void CreateHeap(int capacity,int heap_type){
    //http://man7.org/linux/man-pages/man2/mmap.2.html
    HeapStr = (Heap *)mmap(NULL,sizeof(Heap),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    //check if memory allocation is fails
    if(HeapStr == NULL){
        perror("Heap Memory Error!");
        exit(10);
    }
    HeapStr->heap_type = heap_type;
    HeapStr->count=0;
    HeapStr->capacity = capacity;
    HeapStr->arr = (int *)mmap(NULL,NO_OF_PROCESSES*sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    //check if allocation succeed
    if ( HeapStr->arr == NULL){
        perror("Heap(arr) Memory Error!");
        exit(11);
    }
}

void insert(Heap *h, int key){
    if( h->count < h->capacity){
        h->arr[h->count] = key;
        heapify_bottom_top(h, h->count);
        h->count++;
    }
}

void heapify_bottom_top(Heap *h,int index){
    int temp;
    int parent_node = (index-1)/2;

    if(h->arr[parent_node] > h->arr[index]){
        //swap and recursive call
        temp = h->arr[parent_node];
        h->arr[parent_node] = h->arr[index];
        h->arr[index] = temp;
        heapify_bottom_top(h,parent_node);
    }
}

void heapify_top_bottom(Heap *h, int parent_node){
    int left = parent_node*2+1;
    int right = parent_node*2+2;
    int min;
    int temp;

    if(left >= h->count || left <0)
        left = -1;
    if(right >= h->count || right <0)
        right = -1;

    if(left != -1 && h->arr[left] < h->arr[parent_node])
        min=left;
    else
        min =parent_node;
    if(right != -1 && h->arr[right] < h->arr[min])
        min = right;

    if(min != parent_node){
        temp = h->arr[min];
        h->arr[min] = h->arr[parent_node];
        h->arr[parent_node] = temp;
        // recursive  call
        heapify_top_bottom(h, min);
    }
}

int PopMin(Heap *h){
    int pop;
    if(h->count==0){
        printf("\n__Heap is Empty__\n");
        return -1;
    }
    // replace first node by last and delete last
    pop = h->arr[0];
    h->arr[0] = h->arr[h->count-1];
    h->count--;
    heapify_top_bottom(h, 0);
    return pop;
}
void print(Heap *h){
    int i;
    printf("____________Print Heap_____________\n");
    for(i=0;i< h->count;i++){
        printf("-> %d ",h->arr[i]);
	if ((i+1)%5==0) printf("\n");
    }
    printf("\n__________________________________\n");
}
void print_sorted(Heap *h){
	int i;
	printf("Print Min Heap contents sorted\n");
	for(i=NO_OF_PROCESSES-1;i>=0;i--)
        	printf("Min(%d) : %d\n", NO_OF_PROCESSES-i,PopMin(HeapStr));
}
//**************************End of Heap implementation ***********************************/

void child_process_code(int priority) {
	sem_wait(Sem1);
	insert(HeapStr, priority);
	sem_post(Sem1);
}

int main() { 
    pid_t pid;
    int status,i,prt;
    srand(time(0)); 
	
	Sem1 = sem_open ("Sem1", O_CREAT | O_EXCL, 0644, 1); //last argument is the initial value of the semaphore
   	CreateHeap(NO_OF_PROCESSES, 0); //Min Heap
	
    /* fork child processes */
    for (i = 0; i < NO_OF_PROCESSES; i++){
		prt = (rand() % (UPPER - LOWER + 1)) + LOWER;
        pid = fork ();
        if (pid < 0) {
			/* check for error*/
			sem_unlink ("Sem1");sem_close(Sem1);
            perror("Fork error.\n");
        }
        else if (pid == 0) {
            break;                  /* child processes */
		}
    }
    if (pid==0){
		/******************************************************/
		/******************   CHILD PROCESS   *****************/
		/******************************************************/
		child_process_code(prt);
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
        printf("\nParent: All children have exited.\n");
        /* cleanup semaphores */
        sem_unlink ("Sem1");sem_close(Sem1);
		print(HeapStr);
		print_sorted(HeapStr);
		munmap(HeapStr->arr, NO_OF_PROCESSES*sizeof(int));
		munmap(HeapStr, sizeof(Heap));
        exit(0);
    }
} 
