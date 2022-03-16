??? from here until ???END lines may have been inserted/deleted
DOBJ= code
fibo: ./include/fibo.h    
	cd code/ && make
	gcc -o fibo $(DOBJ)/main.o $(DOBJ)/fibo.o -I./include 
cleaclean:
	$(MAKE) --directory=$(DOBJ) clean
