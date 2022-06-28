#include "lib.h"

int ret = -2;
void *test(void *arg) {
	writef("son is begin\n");
	exit();
	/*
	int i;
	for(i=0; i<10; i++){
		writef("son is running\n");
		syscall_yield();
	}
	writef("success\n");
	*/
}

void umain() {
	writef("Begin\n");
	writef("create pthread\n");
	pthread_t thread;
	int args[3];
	args[0] = 1;
	args[1] = 2;
	args[2] = 3;
	pthread_create(&thread,NULL,test,(void *)args);
	writef("son is create!\n");
	syscall_yield();
	user_panic("wrong");
	pthread_exit(NULL);
}

