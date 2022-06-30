#include "lib.h"
const int exitvalue1 = 100;
const int exitvalue2 = 200;
void *test1(void *arg){
    return &exitvalue1;
}
void *test2(void *arg){
   	pthread_exit(&exitvalue2);
}
void umain(){
    int* getvalue;
    pthread_t son;
    pthread_create(&son, NULL, test1, NULL);
    pthread_join(son, &getvalue);
    if(*getvalue != exitvalue1){
        user_panic("join test failed!");
    }
    pthread_create(&son, NULL, test2, NULL);
    pthread_join(son, &getvalue);
    if(*getvalue != exitvalue2){
        user_panic("join test failed!");
    }
    writef("join test succeeds!\n");
    pthread_exit(NULL);
}
