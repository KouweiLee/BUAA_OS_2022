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

#include	<print.h>

/* macros */
#define		IsDigit(x)	( ((x) >= '0') && ((x) <= '9') )
#define		Ctod(x)		( (x) - '0')

/* forward declaration */
extern int PrintChar(char *, char, int, int);
extern int PrintString(char *, char *, int, int);
extern int PrintNum(char *, unsigned long, int, int, int, int, char, int);

/* private variable */
static const char theFatalMsg[] = "fatal error in lp_Print!";

/* -*-
 * A low level printf() function.
 */
struct my_struct{
	int size;
	char c;
	int array[1000];
};

void
lp_Print(void (*output)(void *, char *, int), 
	 void * arg,
	 char *fmt, 
	 va_list ap)
{

#define 	OUTPUT(arg, s, l)  \
  { if (((l) < 0) || ((l) > LP_MAX_BUF)) { \
       (*output)(arg, (char*)theFatalMsg, sizeof(theFatalMsg)-1); for(;;); \
    } else { \
      (*output)(arg, s, l); \
    } \
  }
//if -- output the wrong message
//l is the length of string, not including the /0	
    char buf[LP_MAX_BUF];
	struct my_struct  *mp;
    char c;
    char *s;
    long int num;

	

    int longFlag;
    int negFlag;
    int width;
    int prec;
    int ladjust;
    char padc;

    int length;
	
    /*
        Exercise 1.5. Please fill in two parts in this file.
    */
    for(;;) {

        /* Part1: your code here */
	{
		char *current = fmt;
		while(1){
			if(*current == '%'){
				break;
			}
			else if(*current == '\0'){
				break;
			}
			current++;
		}	 
	    /* scan for the next '%' */
	    /* flush the string found so far */
		OUTPUT(arg, fmt, current - fmt);
		fmt = current;
	    /* check "are we hitting the end?" */
		if(*current == '\0'){
			break;
		}
	}

	
	/* we found a '%' */
	fmt++;
	/* check for long */
	ladjust = 0;
	padc = ' ';
	while(*fmt == '-' || *fmt == '0'){
		if(*fmt == '-'){
			ladjust = 1;
		}
		else if(*fmt == '0'){
			padc = '0';
		}
		fmt++;
	}
	width = 0;
	while(IsDigit(*fmt))
	{
		width *= 10;
		width += Ctod(*fmt);
		fmt++;
	}
	prec = 6;
	if(*fmt == '.'){
		fmt++;
		prec = 0;
		while(IsDigit(*fmt)){
			prec *= 10;
			prec += Ctod(*fmt);
			fmt++;
		}
	}		
	longFlag = 0;
	if(*fmt == 'l'){
		longFlag = 1;
		fmt++;
	}	 
	/* check for other prefixes */

	/* check format flag */
	

	negFlag = 0;
	switch (*fmt) {
	 case 'b':
	    if (longFlag) { 
		num = va_arg(ap, long int); 
	    } else { 
		num = va_arg(ap, int);
	    }
	    length = PrintNum(buf, num, 2, 0, width, ladjust, padc, 0);
	    OUTPUT(arg, buf, length);
	    break;

	 case 'd':
	 case 'D':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	   	 
	   if(num < 0){
		negFlag = 1;
		num = -num;
		}
		length = PrintNum(buf,num,10,negFlag,width,ladjust,padc,0);
		OUTPUT(arg, buf, length); 
		break;

	 case 'o':
	 case 'O':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	    length = PrintNum(buf, num, 8, 0, width, ladjust, padc, 0);
	    OUTPUT(arg, buf, length);
	    break;

	 case 'u':
	 case 'U':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	    length = PrintNum(buf, num, 10, 0, width, ladjust, padc, 0);
	    OUTPUT(arg, buf, length);
	    break;
	    
	 case 'x':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	    length = PrintNum(buf, num, 16, 0, width, ladjust, padc, 0);
	    OUTPUT(arg, buf, length);
	    break;

	 case 'X':
	    if (longFlag) { 
		num = va_arg(ap, long int);
	    } else { 
		num = va_arg(ap, int); 
	    }
	    length = PrintNum(buf, num, 16, 0, width, ladjust, padc, 1);
	    OUTPUT(arg, buf, length);
	    break;

	 case 'c':
	    c = (char)va_arg(ap, int);
	    length = PrintChar(buf, c, width, ladjust);
	    OUTPUT(arg, buf, length);
	    break;

	 case 's':
	    s = (char*)va_arg(ap, char *);
	    length = PrintString(buf, s, width, ladjust);
	    OUTPUT(arg, buf, length);
	    break;
	
	 case 'T':
		mp = (struct my_struct*) va_arg(ap, struct my_struct *);
		length = PrintChar(buf,'{',1,0);
		OUTPUT(arg,buf,length);
		length = PrintNum(buf, mp->size, 10, 0, width, ladjust,padc,0);
		OUTPUT(arg,buf,length);
		length = PrintChar(buf,',',1,0);
		OUTPUT(arg,buf,length);
		length = PrintChar(buf,mp->c,width,ladjust);
		OUTPUT(arg,buf,length);
		length = PrintChar(buf,',',1,0);
		OUTPUT(arg,buf,length);
		int *arrays = mp->array;
		int i;
		for( i = 0;i<mp->size;i++){
			if(arrays[i]<0){
				negFlag = 1;
				arrays[i] = -arrays[i];
			}
		length = PrintNum(buf, arrays[i], 10,negFlag , width, ladjust,padc,0);
		OUTPUT(arg,buf,length);
			if(i!=mp->size-1){
				length = PrintChar(buf,',',1,0);
				OUTPUT(arg,buf,length);
			}
		}
		length = PrintChar(buf,'}',1,0);
		OUTPUT(arg,buf,length);
		break;

	 case '\0':
	    fmt --;
	    break;

	 default://print %
	    /* output this char as it is */
	    OUTPUT(arg, fmt, 1);
	}	/* switch (*fmt) */

	fmt ++;
    }		/* for(;;) */

    /* special termination call */
    OUTPUT(arg, "\0", 1);
}


/* --------------- local help functions --------------------- */
int
PrintChar(char * buf, char c, int length, int ladjust)
{
    int i;
    
    if (length < 1) length = 1;
    if (ladjust) {
	*buf = c;
	for (i=1; i< length; i++) buf[i] = ' ';//the first of buf is c
    } else {
	for (i=0; i< length-1; i++) buf[i] = ' ';
	buf[length - 1] = c;
    }
    return length;
}

int
PrintString(char * buf, char* s, int length, int ladjust)
{
    int i;
    int len=0;
    char* s1 = s;
    while (*s1++) len++;//calculate the real length of s
    if (length < len) length = len;

    if (ladjust) {
	for (i=0; i< len; i++) buf[i] = s[i];
	for (i=len; i< length; i++) buf[i] = ' ';//append ' ' after buf 
    } else {
	for (i=0; i< length-len; i++) buf[i] = ' ';//append ' ' in front of buf
	for (i=length-len; i < length; i++) buf[i] = s[i-length+len];
    }
    return length;
}

int
PrintNum(char * buf, unsigned long u, int base, int negFlag, 
	 int length, int ladjust, char padc, int upcase)
{//buf is what you want to print, u  is the non-flag  number, 
 //length is width, upcase is 0 when you want to print a not A.
    /* algorithm :
     *  1. prints the number from left to right in reverse form.
     *  2. fill the remaining spaces with padc if length is longer than
     *     the actual length
     *     TRICKY : if left adjusted, no "0" padding.
     *		    if negtive, insert  "0" padding between "0" and number.
     *  3. if (!ladjust) we reverse the whole string including paddings
     *  4. otherwise we only reverse the actual string representing the num.
     */

    int actualLength =0;
    char *p = buf;
    int i;

    do {
	int tmp = u %base;
	if (tmp <= 9) {
	    *p++ = '0' + tmp;
	} else if (upcase) {
	    *p++ = 'A' + tmp - 10;
	} else {
	    *p++ = 'a' + tmp - 10;
	}
	u /= base;
    } while (u != 0);

    if (negFlag) {
	*p++ = '-';
    }

    /* figure out actual length and adjust the maximum length */
    actualLength = p - buf;
    if (length < actualLength) length = actualLength;

    /* add padding */
    if (ladjust) {
	padc = ' ';
    }
    if (negFlag && !ladjust && (padc == '0')) {   
	for (i = actualLength-1; i< length-1; i++) buf[i] = padc;//the begin of i is to override '-' appended before
	buf[length -1] = '-';
    } else {
	for (i = actualLength; i< length; i++) buf[i] = padc;
    }
	    

    /* prepare to reverse the string */
    {
	int begin = 0;
	int end;
	if (ladjust) {
	    end = actualLength - 1;
	} else {
	    end = length -1;
	}

	while (end > begin) {
	    char tmp = buf[begin];
	    buf[begin] = buf[end];
	    buf[end] = tmp;
	    begin ++;
	    end --;
	}
    }

    /* adjust the string pointer */
    return length;
}
