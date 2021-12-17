CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3 -lm

all: TSP Unit

TSP: greedy.o lugraph.o queue.o location.o
	${CC} ${CFLAGS} -o $@ $^

Unit: lugraph.o location.o lugraph_unit.c queue.o
	${CC} ${CFLAGS} -o $@ $^

greedy.o: greedy.c 
	${CC} ${CFLAGS} -c greedy.c

lugraph.o: lugraph.h lugraph.c 
	${CC} ${CFLAGS} -c lugraph.c

queue.o: queue.h queue.c 
	${CC} ${CFLAGS} -c queue.c

location.o: location.c location.h
	${CC} ${CFLAGS} -c location.c

clean:
	rm -r *.o Unit