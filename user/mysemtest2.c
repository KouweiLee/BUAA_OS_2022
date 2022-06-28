#include "lib.h"
sem_t signal;
#define E_SEM_ERROR 16
void umain(){
    int r, num = 10, value;
    sem_init(&signal, 0, num);
    sem_getvalue(&signal, &value);
    if(value != num) user_panic("getvalue is wrong");
    sem_destroy(&signal);
    if((r = sem_wait(&signal)) != -E_SEM_ERROR) 
        user_panic("destroy is wrong");
    pthread_exit(NULL);
}
