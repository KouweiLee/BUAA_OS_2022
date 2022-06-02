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
int time_read(){
	int addr = 0x15000000;
    int offset = 0x0010;
    int time;
    syscall_write_dev(&time, addr, 4);
    syscall_read_dev(&time, addr+offset, 4);
    return time;
}
void raid0_write(u_int secno, void *src, u_int nsecs){
	u_int diskno;
	u_int nowsec;
	for(nowsec = secno; nowsec < secno + nsecs; nowsec++){
		if(nowsec%2==0){
			diskno = 1;
		}else {
			diskno = 2;
		}
		ide_write(diskno, nowsec/2, src+(nowsec-secno)*0x200, 1);
	}
}
void raid0_read(u_int secno, void *dst, u_int nsecs){
	u_int diskno;
	u_int nowsec;
	for(nowsec = secno; nowsec < secno + nsecs; nowsec++){
		if(nowsec%2==0){
			diskno = 1;
		}else {
			diskno = 2;
		}
		ide_read(diskno, nowsec/2, dst+(nowsec-secno)*0x200, 1);
	}
}
