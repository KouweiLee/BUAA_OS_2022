#include "lib.h"
void umain() {
	int fd = open("/motd", O_RDWR);
	char buf[128];
	user_bzero(buf, 128);
	read(fd, buf, 10);
	if(fork() == 0) {
		//son
		char bufSon[128];
		user_bzero(bufSon, 128);
		read(fd, bufSon, 10);
		writef("son's buf is %s\n", bufSon);
	}else {
		char bufFa[128];
        user_bzero(bufFa, 128);
        read(fd, bufFa, 10);
        writef("father's buf is %s\n", bufFa);
	}
}
