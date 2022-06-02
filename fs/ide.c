/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int r;
	u_int now;
	u_int zero = 0;
	while (offset_begin + offset < offset_end) {
		now = offset_begin + offset;
		if(syscall_write_dev(&diskno, 0x13000010, 4) < 0) 
			user_panic("ide_read_panic!");
		if(syscall_write_dev(&now, 0x13000000, 4) < 0)
			user_panic("ide_read_panic!");
		if(syscall_write_dev(&zero, 0x13000020, 4) < 0)
			user_panic("ide_read_panic!");
		if(syscall_read_dev(&r, 0x13000030, 4) < 0)
			user_panic("ide_read_panic!");
		if(r == 0) 
			user_panic("ide_read_panic!");
		if(syscall_read_dev((u_int)(dst+offset),0x13004000, 0x200) < 0)
			user_panic("ide_read_panic!");
		offset+=0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	// Your code here
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int r;
	u_int one = 1;
	u_int now;
	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
		now = offset_begin + offset;
        if(syscall_write_dev(&diskno, 0x13000010, 4) < 0)
            user_panic("ide_write_panic!");
        if(syscall_write_dev(&now, 0x13000000, 4) < 0)
            user_panic("ide_write_panic!");
		if(syscall_write_dev((u_int)(src+offset),0x13004000, 0x200) < 0)
            user_panic("ide_write_panic!");
        if(syscall_write_dev(&one, 0x13000020, 4) < 0)
            user_panic("ide_write_panic!");
        if(syscall_read_dev(&r, 0x13000030, 4) < 0)
            user_panic("ide_write_panic!");
        offset+=0x200;
		// if error occur, then panic.
	}
}

int raid4_valid(u_int diskno){
	u_int now = 0;
	u_int zero = 0;
	int r;
	syscall_write_dev(&diskno, 0x13000010, 4);
	syscall_write_dev(&now, 0x13000000, 4);
	syscall_write_dev(&zero, 0x13000020, 4);
	syscall_read_dev(&r, 0x13000030, 4);
	if(r == 0){//´æÔÚÒì³£
	return 0;
	}
	return 1;
}

int raid4_write(u_int blockno, void *src){
	char *begin = (char*) src;
	u_int secno = 2*blockno;
	int i, r = 0;
	//char a0[512] = begin;
	//char a1[512] = being + 0x200;
	char p0[512], p1[512];
	for(i=0;i<512;i++){
		p0[i] = begin[i] ^ begin[i+0x200] ^ begin[i+0x200*2] ^ begin[i+0x200*3];
		p1[i] = begin[i+0x200*4] ^ begin[i+0x200*5] ^ begin[i+0x200*6] ^ begin[i+0x200*7];
	}
	for(i=1;i<=4;i++){
		if(raid4_valid(i)){
			ide_write(i, secno, begin+0x200*(i-1), 1);
			ide_write(i, secno+1, begin+0x200*(i+3), 1);
		}else {
			r++;
		}
	}
	if(raid4_valid(5)){
		ide_write(5, secno, p0, 1);
		ide_write(5, secno+1, p1, 1);
	}else {
		r++;
	}
	return r;
}

int raid4_read(u_int blockno, void *dst){
	char *begin = (char*)dst;
	u_int secno = 2*blockno;
	int i, r=0;
	char p0[512], p1[512];
	for(i=1;i<=5;i++){
		if(!raid4_valid(i)){
			r++;
		}
	}
	if(r>0){
		return r;
	}
	//ÎÞ´ÅÅÌËð»µ
	for(i=1;i<=4;i++){
		ide_read(i, secno, begin+0x200*(i-1), 1);
		ide_read(i, secno+1, begin + 0x200*(i+3), 1);
	}
	user_bzero((void *)p0, 512);
	user_bzero((void *)p1, 512);
	ide_read(5, secno, p0, 1);
	ide_read(5, secno+1, p1, 1);
	for(i=0;i<512;i++){
		char aa = begin[i] ^ begin[i+0x200] ^ begin[i+0x200*2] ^ begin[i+0x200*3];
		char ab = begin[i+0x200*4] ^ begin[i+0x200*5] ^ begin[i+0x200*6] ^ begin[i+0x200*7];
		if(!(p0[i] == aa && p1[i] == ab)){
			return -1;
		}
	}
	return 0;
}
