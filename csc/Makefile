DOBJ= code
fibo: ./include/fibo.h
	cd code/ && make
	gcc -o fibo $(DOBJ)/main.o $(DOBJ)/fibo.o -I./include 
clean:
	$(MAKE) --directory=$(DOBJ) clean	
