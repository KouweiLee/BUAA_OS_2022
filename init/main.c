/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <pmap.h>
struct my_struct {
	int size;
	char c;
	int array[10];
};
int main()
{   
	struct my_struct t1;
struct my_struct t2;
t1.size=3; t1.c='b'; t1.array[0]=0; t1.array[1]=1; t1.array[2]=2;
t2.size=2; t2.c='Q'; t2.array[0]=-1; t2.array[1]=-2;
printf("%04T%d%T\n",&t1,123,&t2);	
printf("%04T%d%T\n",&t2,123,&t1);
printf("main.c:\tmain is start ...\n");
	mips_init();
	panic("main is over is error!");

	//printf("my name is %024s, he is a %-24s, his age is %-12.4ld\n", "cjj", "boy", 20);
	return 0;
}
