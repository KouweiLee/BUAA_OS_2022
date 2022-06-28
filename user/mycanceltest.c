#include "lib.h"
void *test1(int *flag){//for default cancel test
    while((&flag) == 0){
        syscall_yield();
    }
    writef("son begins\n");
    pthread_testcancel();
    user_panic("son hasn't exit");
}

void *test2(int *flag){//for disable cancelstate test
    int oldvalue;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldvalue);
    if(oldvalue != PTHREAD_CANCEL_ENABLE)
        user_panic("cancelstate is wrong");
    *flag = 1;
	pthread_testcancel();
	writef("test2 success!\n");
}         

void *test3(int *flag){
	int oldvalue;
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldvalue);
	if(oldvalue != PTHREAD_CANCEL_DEFERRED)
		user_panic("canceltype is wrong");
	*flag = 1;
	syscall_yield();
	user_panic("son3 hasn't been canceled");
}
void umain(){
    pthread_t son;
    int *value_ptr, r, flag = 0, value = 0;
    value_ptr = &value;
	//for test1
    pthread_create(&son, NULL, test1, (void *)(&flag));
    writef("create son1!\n");
    if((r = pthread_cancel(son)) < 0) 
        user_panic("pthread_cancel is wrong");
    flag = 1;
    pthread_join(son, &value_ptr);
    if(*value_ptr != 99) 
       	user_panic("return value is wrong!");
    //for test2
    flag = 0;
    pthread_create(&son, NULL, test2, (void *)&flag);
    writef("create son2!\n");
    while(flag == 0){
        syscall_yield();
    }
    if((r = pthread_cancel(son)) == 0) 
        user_panic("pthread_cancel is wrong");
	//for test3
	flag = 0;
	pthread_create(&son, NULL, test3, (void *)&flag);
	writef("create son3!\n");
	while(flag == 0) {
		syscall_yield();
	}
	if((r = pthread_cancel(son)) < 0)
		user_panic("pthread_cancel is wrong");
    pthread_exit(NULL);
}
