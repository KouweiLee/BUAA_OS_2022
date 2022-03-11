VPATH=../include
fibo: fibo.c main.c fibo.h
	gcc -c fibo.c -I../include
	gcc -c main.c -I../include
	gcc fibo.o main.o -o fibo -I../include
	mv fibo ../
clean:
	rm fibo.o main.o 
