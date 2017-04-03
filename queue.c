/* Queue code for Computer Systems Project 1
 * Author: Kaven Peng kavenp 696573
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "qnode.h"

QNode_t
*newQNode(process_t *process)
{
	QNode_t *new = (QNode_t*)malloc(sizeof(QNode_t));
	if(new == NULL) {
		fprintf(stderr, "Error during malloc.\n");
		exit(1);
	}
	new->process = process;
	new->next = NULL;
	return new;
}

Queue_t
*createQueue()
{
	Queue_t *q = (Queue_t*)malloc(sizeof(Queue_t));
	if(q == NULL) {
		fprintf(stderr, "Error during malloc.\n");
		exit(1);
	}
	q->front = NULL;
	q->rear = NULL;
	return q;
}

int
isEmptyQ(Queue_t *q)
{
	if(q->front == NULL && q->rear == NULL) {
		return 1;
	}
	return 0;
}

void
enQueue(Queue_t *q, process_t *process)
{
	QNode_t *new = newQNode(process);

	//if queue empty, then new node is front and rear
	if(q->rear == NULL) {
		q->front = q->rear = new;
		return;
	}
	//add new node to end and change rear
	q->rear->next = new;
	q->rear = new;
}

QNode_t*
deQueue(Queue_t *q)
{
	QNode_t *temp;
	//if queue empty return NULL
	if(q->front == NULL) {
		return NULL;
	}
	//store front and move down
	temp = q->front;
	q->front = q->front->next;
	//if front becomes NULL, rear also becomes NULL
	if(q->front == NULL) {
		q->rear = NULL;
	}
	return temp;
}

void
printQ(Queue_t *q)
{
	QNode_t *current = q->front;
	while(current != NULL) {
		printf("Process data: %d %d %d %d\n", current->process->timeCreate, current->process->processId
			, current->process->size, current->process->runtime);
		current = current->next;
	}
}

void 
freeQ(Queue_t *q)
{
	QNode_t *curr, *prev;
	if(q != NULL) {
		curr = q->front;
		while (curr != NULL) {
			prev = curr;
			curr = curr->next;
			free(prev);
		}
	}
}