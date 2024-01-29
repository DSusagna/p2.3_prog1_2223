all: p3

p3: main.o linkedlist.o
	gcc main.o linkedlist.o -o p3

main.o: main.c linkedlist.h
	gcc -c main.c 

linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c 

clean:
	rm *.o
	rm p3

