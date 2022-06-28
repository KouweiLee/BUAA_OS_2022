#include "lib.h"
#include <mmu.h>
#include <env.h>
void son_exit(void){

	struct Tcb *t = &env->env_threads[TCBX(syscall_getthreadid())];
	t->tcb_exit_ptr = 0;
	syscall_thread_destroy(0);
}

void exit(void)
{
	/*struct Tcb *t = &env->env_threads[TCBX(syscall_getthreadid())];
	t->tcb_exit_ptr = 0;
	syscall_thread_destroy(0);
	*/
	syscall_env_destroy(0);
}


struct Env *env;
struct Tcb *tcb;

void libmain(int argc, char **argv)
{
	// set env to point at our env structure in envs[].
	env = 0;	// Your code here.
	tcb = 0;
	//writef("xxxxxxxxx %x  %x  xxxxxxxxx\n",argc,(int)argv);
	int envid;
	envid = syscall_getenvid();
	envid = ENVX(envid);
	env = &envs[envid];
	int tcbid;
	tcbid = syscall_getthreadid();
	tcbid = TCBX(tcbid);
	tcb = &env->env_threads[tcbid];
	// call user main routine
	umain(argc, argv);
	// exit gracefully
	exit();
}
