#include "lib.h"
int data = 100;
#define E_SEM_EAGAIN 18
sem_t mutex;
//test3 is for trywait
void *test3(void *arg){
	int b, i, tmp, r;
    while((r = sem_trywait(&mutex)) == -E_SEM_EAGAIN){
		syscall_yield();
	}
    tmp = data;
    for(i=0;i<100;i++){
        b = data;
        b = b - 1;
        data = b;
        syscall_yield();
    }
    if(data != tmp - 100)
        user_panic("mutex is wrong");
    sem_post(&mutex);
    writef("test3 succeeds\n");
}
//test1 and test2 are for wait and post test
void *test2(void *arg){
    int b, i, tmp;
    sem_wait(&mutex);
    tmp = data;
    for(i=0;i<100;i++){
        b = data;
        b = b - 1;
        data = b;
		syscall_yield();
    }
    if(data != tmp - 100)
        user_panic("mutex is wrong");
    sem_post(&mutex);
	writef("test2 succeeds\n");
}

void *test1(void *arg){
    int a, i, tmp;
    sem_wait(&mutex);
    tmp = data;
    for(i=0;i<100;i++){
        a = data;
        a = a + 1;
        data = a;
		syscall_yield();
    }
    if(data != tmp + 100)
        user_panic("mutex is wrong");
    sem_post(&mutex);
	writef("test1 succeeds\n");
}

void umain(){
    pthread_t son[15];
    int i, *value;
	sem_init(&mutex, 0, 1);
    pthread_create(&son[0], NULL, test1, NULL);
    pthread_create(&son[1], NULL, test2, NULL);
	pthread_join(son[0], NULL);
	pthread_join(son[1], NULL);
	for(i=0; i<15; i++){
		pthread_create(&son[i], NULL, test1, NULL);
	}
	for(i=0; i<15; i++){
		pthread_join(son[i], NULL);
	}	

	for(i=0; i<15; i++){
		pthread_create(&son[i], NULL, test3, NULL);
	}
    pthread_exit(NULL);
}
