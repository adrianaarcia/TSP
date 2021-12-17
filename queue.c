#include "queue.h"

/*
* Aspnes's queue implementation. Taken from: http://www.cs.yale.edu/homes/aspnes/classes/223/examples/linkedLists/queue.c
*/


/* standard linked list element */
struct elt {
    elt *next;
    int value;
};

struct queue {
    elt *head;  /* dequeue this next */
    elt *tail;  /* enqueue after this */
};

/* create a new empty queue */
queue * queueCreate(void)
{
    queue *q;

    q = malloc(sizeof(queue));

    q->head = q->tail = 0;

    return q;
}

/* add a new value to back of queue */
void enq(queue *q, int value)
{
    elt *e;

    e = malloc(sizeof(elt));
    assert(e);

    e->value = value;

    /* Because I will be the tail, nobody is behind me */
    e->next = 0;

    if(q->head == 0) {
        /* If the queue was empty, I become the head */
        q->head = e;
    } else {
        /* Otherwise I get in line after the old tail */
        q->tail->next = e;
    }

    /* I become the new tail */
    q->tail = e;
}

int queueEmpty(const queue *q)
{
    return (q->head == 0);
}

/* remove and return value from front of queue */
int deq(queue *q)
{
    int ret;
    elt *e;

    assert(!queueEmpty(q));

    ret = q->head->value;

    /* patch out first element */
    e = q->head;
    q->head = e->next;

    free(e);

    return ret;
}

/* free a queue and all of its elements */
void queueDestroy(queue *q)
{
    while(!queueEmpty(q)) {
        deq(q);
    }

    free(q);
}
