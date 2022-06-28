#include <env.h>
#include <pmap.h>
#include <printf.h>

void sched_yield(void)
{
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current tcb_sched_list index
    static struct Tcb *t = NULL;
    int cempty=0;
	//printf("exe\n");
    if(count == 0 || t == NULL || t->tcb_status != ENV_RUNNABLE){
        if(t != NULL){//count == 0 || status is not runnable
            LIST_REMOVE(t, tcb_sched_link);
            if(t->tcb_status == ENV_RUNNABLE){
                LIST_INSERT_TAIL(&tcb_sched_list[1-point], t, tcb_sched_link);
            }
        }
        while(1){
            while(LIST_EMPTY(&tcb_sched_list[point])){ 
                point = 1 - point;
				cempty++;
				if(cempty>2) panic("no threads!");
			}
            t = LIST_FIRST(&tcb_sched_list[point]);
            if(t->tcb_status == ENV_FREE){
                LIST_REMOVE(t, tcb_sched_link);
            } else if(t->tcb_status == ENV_NOT_RUNNABLE){
                LIST_REMOVE(t, tcb_sched_link);
                //LIST_INSERT_TAIL(&tcb_sched_list[1-point], t, tcb_sched_link);
            } else {
                count = t->tcb_pri;
                break;
            }
        }
    }
    count --;
    env_run(t);
}

/* Overview:
 *  Implement simple round-robin scheduling.
 *
 *
 * Hints:
 *  1. The variable which is for counting should be defined as 'static'.
 *  2. Use variable 'env_sched_list', which is a pointer array.
 *  3. CANNOT use `return` statement!
 */
/*** exercise 3.15 ***/
/*
void sched_yield(void)
{
	
    static int count = 0; // remaining time slices of current env
    static int point = 0; // current env_sched_list index
    static struct Env *e = NULL;
	if(count == 0 || e == NULL || e->env_status != ENV_RUNNABLE){
		if(e != NULL){//count == 0 || status is not runnable
			LIST_REMOVE(e, env_sched_link);
			if(e->env_status != ENV_FREE){
				LIST_INSERT_TAIL(&env_sched_list[1-point], e, env_sched_link);
			}
		}
		while(1){
			while(LIST_EMPTY(&env_sched_list[point])) 
				point = 1 - point;
			e = LIST_FIRST(&env_sched_list[point]);
			if(e->env_status == ENV_FREE){
				LIST_REMOVE(e, env_sched_link);
			} else if(e->env_status == ENV_NOT_RUNNABLE){
				LIST_REMOVE(e, env_sched_link);
				LIST_INSERT_TAIL(&env_sched_list[1-point], e, env_sched_link);
			} else {
				count = e->env_pri;
				break;
			}
		}
	}

	count --;
	env_run(e);
	//env_run(LIST_FIRST(&env_sched_list[0]));
}
*/

