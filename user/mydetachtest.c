#include "lib.h"
void *test(void *arg){
    writef("son begins\n");
    syscall_yield();
    writef("son ends\n");
}

void umain(){
    pthread_t son;
    pthread_create(&son, NULL, test, NULL);
    pthread_detach(son);
    int r = pthread_join(son, NULL);
    if(r == 0) user_panic("detach is wrong");
	writef("father ends");
    pthread_exit(NULL);
}
