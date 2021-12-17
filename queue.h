#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
* Aspnes's queue implementation. Taken from: http://www.cs.yale.edu/homes/aspnes/classes/223/examples/linkedLists/queue.c
*/


/* standard linked list element */
typedef struct elt elt;
typedef struct queue queue;

/* create a new empty queue */
queue * queueCreate(void);

/* add a new value to back of queue */
void enq(queue *q, int value);

int queueEmpty(const queue *q);

/* remove and return value from front of queue */
int deq(queue *q);

/* free a queue and all of its elements */
void queueDestroy(queue *q);