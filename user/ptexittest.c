#include "lib.h"

int ret = -2;
void *test(void *arg) {
	writef("son is begin\n");
	writef("son threadid is 0x%x\n",syscall_getthreadid());
	pthread_exit(&ret);
	writef("son not exit!\n");
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
	
	while ((int)env->env_threads[TCBX(thread)].tcb_status != ENV_FREE); 
	writef("\n");
	writef("son exit ret is %d\n",*((int *)env->env_threads[TCBX(thread)].tcb_exit_ptr));
	
}

