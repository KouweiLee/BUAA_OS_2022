// User-level IPC library routines

#include "lib.h"
#include <mmu.h>
#include <env.h>

extern struct Env *env;
extern void (*__kill_handler)(int);
//extern void __asm_pgfault_handler(void);
void kill(u_int envid, int sig){
	if(envid == 0 || envid == env->env_id){
		if(__kill_handler == 0){
			exit();
		}else {
			__kill_handler(15);
		}
	} else{
		syscall_kill(envid, sig);
	}
	//writef("success finished\n");
//	syscall_kill(envid, sig);
}

void signal(int sig, void (*handler)(int)){
	if(sig == 15){
	__kill_handler = handler;//NULL???
	syscall_set_kill_handler((u_int)handler);
	syscall_mem_alloc(0, UXSTACKTOP - BY2PG, PTE_V | PTE_R);
	}
}
// Send val to whom.  This function keeps trying until
// it succeeds.  It should panic() on any error other than
// -E_IPC_NOT_RECV.
//
// Hint: use syscall_yield() to be CPU-friendly.
void
ipc_send(u_int whom, u_int val, u_int srcva, u_int perm)
{
	int r;

	while ((r = syscall_ipc_can_send(whom, val, srcva, perm)) == -E_IPC_NOT_RECV) {
		syscall_yield();
		//writef("QQ");
	}

	if (r == 0) {
		return;
	}

	user_panic("error in ipc_send: %d", r);
}

// Receive a value.  Return the value and store the caller's envid
// in *whom.
//
// Hint: use env to discover the value and who sent it.
u_int
ipc_recv(u_int *whom, u_int dstva, u_int *perm)
{
	//printf("ipc_recv:come 0\n");
	syscall_ipc_recv(dstva);

	if (whom) {
		*whom = env->env_ipc_from;
	}

	if (perm) {
		*perm = env->env_ipc_perm;
	}

	return env->env_ipc_value;
}

