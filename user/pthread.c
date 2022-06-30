#include "lib.h"
#include <error.h>
#include <mmu.h>
extern u_int pthread_cancel_value;
#define PTHREAD_CANCELED (void *)(&pthread_cancel_value)
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void * (*start_rountine)(void *), void *arg) {
	int newthread = syscall_thread_alloc();
	if (newthread < 0) {
		*thread = 0;
		return newthread;
	}
	struct Tcb *t = &env->env_threads[TCBX(newthread)];
	t->tcb_tf.regs[29] = USTACKTOP - TCB_STACK(TCBX(newthread));
	t->tcb_tf.pc = start_rountine;
	t->tcb_tf.regs[29] -= 4;//according to MIPS, parameter should be placed in stack
	t->tcb_tf.regs[4] = arg;//a0 stores arg
	t->tcb_tf.regs[31] = son_exit;//when function finished, return to son_exit
	syscall_set_thread_status(t->tcb_id,ENV_RUNNABLE);
	*thread = t->tcb_id;
	return 0;
}

void pthread_exit(void *value_ptr) {
	u_int threadid = pthread_self();
	struct Tcb *t = &env->env_threads[TCBX(threadid)];
	t->tcb_exit_ptr = value_ptr;
	syscall_thread_destroy(threadid);
}

int pthread_cancel(pthread_t thread) {
	struct Tcb *t = &env->env_threads[TCBX(thread)];
	if (t->tcb_id != thread || t->tcb_status == ENV_FREE) {
		return -E_BAD_TCB;
	}
	if (t->tcb_cancelstate == PTHREAD_CANCEL_DISABLE) {//禁止取消
		return -E_THREAD_CANNOTCANCEL;
	}
	t->tcb_exit_ptr = PTHREAD_CANCELED;
	if (t->tcb_canceltype == PTHREAD_CANCEL_ASYNCHRONOUS) {//立即取消
		syscall_thread_destroy(thread);
	} else {
		t->tcb_canceled = 1;//收到取消信号，直到cancellation point执行
	}
	return 0;
}

u_int pthread_self() {
	return syscall_getthreadid();
}

int pthread_setcancelstate(int state, int *oldvalue) {
	u_int threadid = pthread_self();
	struct Tcb *t = &env->env_threads[TCBX(threadid)];
	if ((state != PTHREAD_CANCEL_ENABLE) & (state != PTHREAD_CANCEL_DISABLE)) {
		return -E_INVAL;
	}
	if (t->tcb_id != threadid) {
		return -E_BAD_TCB;
	}
	if (oldvalue != 0) {
		*oldvalue = t->tcb_cancelstate;
	}
	t->tcb_cancelstate = state;
	return 0;
}

int pthread_setcanceltype(int type, int *oldvalue) {
	u_int threadid = pthread_self();
	struct Tcb *t = &env->env_threads[TCBX(threadid)];
	if (type != PTHREAD_CANCEL_ASYNCHRONOUS && type != PTHREAD_CANCEL_DEFERRED) {
		return -E_INVAL;
	}
	if (t->tcb_id != threadid) {
		return -E_BAD_TCB;
	}
	if (oldvalue != 0) {
		*oldvalue = t->tcb_canceltype;
	}
	t->tcb_canceltype = type;
	return 0;
}

void pthread_testcancel() {
	u_int threadid = pthread_self();
	struct Tcb *t = &env->env_threads[TCBX(threadid)];
	if (t->tcb_id != threadid) {
		user_panic("panic at pthread_testcancel!\n");
	}
	if (t->tcb_canceled && t->tcb_cancelstate == PTHREAD_CANCEL_ENABLE) {
		t->tcb_exit_ptr = PTHREAD_CANCELED;
		syscall_thread_destroy(t->tcb_id);
	}
}

int pthread_join(pthread_t thread, void **value_ptr) {
	int r = syscall_thread_join(thread,value_ptr);
	return r;
}

int pthread_detach(pthread_t thread) {
	struct Tcb *t = &env->env_threads[TCBX(thread)];
	int r;
	int i;
	if (t->tcb_id != thread) {
		return -E_BAD_TCB;
	}
	if (t->tcb_status == ENV_FREE) {
		u_int sp = USTACKTOP - TCB_STACK(TCBX(thread));
		t->tcb_exit_ptr = 0;
		for(i = 1; i <= TCB_SNUM; ++i) {
			r = syscall_mem_unmap(0,sp-i*BY2PG);
			if (r < 0)
				return r;
		}
		user_bzero(t,sizeof(struct Tcb));
	} else {
		t->tcb_detach = 1;
	}
	return 0;
}



