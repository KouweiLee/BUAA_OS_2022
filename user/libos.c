#include "lib.h"
#include <mmu.h>
#include <env.h>

void
exit(void)
{
	//close_all();
	syscall_env_destroy(0);
}
//extern void (*__pgfault_handler)(u_int);
extern void __kill_fenfa(void);

struct Env *env;

void
libmain(int argc, char **argv)
{
	// set env to point at our env structure in envs[].
	env = 0;	// Your code here.
//	writef("libmain: xxxxxxxxx %x  %x  xxxxxxxxx\n",argc,(int)argv);
//	Attention : writef is a user function
	int envid;
	envid = syscall_getenvid();
	envid = ENVX(envid);
	env = &envs[envid];
	syscall_set_kill_fenfa((u_int)__kill_fenfa);
	// call user main routine
	umain(argc, argv);
	// exit gracefully
	exit();
	//syscall_env_destroy(0);
}
