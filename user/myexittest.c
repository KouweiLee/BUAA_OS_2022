#include "lib.h"
void *test(void *args){
    exit();
    user_panic("son doesn't exit");
}

void umain(){
    pthread_t son;
    writef("father begins\n");
    pthread_create(&son, NULL, test, NULL);
    writef("create son!\n");
    syscall_yield();
    user_panic("father doesn't exit!");
    pthread_exit(NULL);
}
