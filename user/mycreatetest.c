#include "lib.h"
sem_t mutex;
void *test(int *args){
    pthread_t threadid = pthread_self();
    int i;
	int sonid = TCBX(threadid);
    writef("son%d is begin\n", TCBX(threadid));
    writef("son%d's arg is %d\n",TCBX(threadid),args[TCBX(threadid)]);
    sem_wait(&mutex); 
	args[0]--;
	sem_post(&mutex);
	while(1){
		sem_wait(&mutex);
		if(args[0]==0){
		sem_post(&mutex);
		break;
		}
		sem_post(&mutex);
		syscall_yield();
	}
}

void umain(){
    pthread_t threads[15], tmp;
    int args[16];
    int i, r;
	sem_init(&mutex, 0, 1);
    for(i=0;i<16;i++){
        args[i] = i;
    }
	args[0] = 15;
    for(i=0; i<15;i++){
        r = pthread_create(&threads[i], NULL, test, (void *)args);
		if(r < 0)
			user_panic("create is wrong");
        syscall_yield();
    }
    pthread_exit(NULL);
}
