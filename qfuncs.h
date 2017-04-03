/* Queue function header for Computer Systems Project 1
 * Author: Kaven Peng kavenp 696573
 *
 */

#ifndef __QFUNCS__
#define __QFUNCS__

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "qnode.h"

QNode_t *newQNode(process_t *process);
Queue_t *createQueue(void);
int isEmptyQ(Queue_t *q);
void enQueue(Queue_t *q, process_t *process);
QNode_t* deQueue(Queue_t *q);
void printQ(Queue_t *q);
void freeQ(Queue_t *q);

#endif //qfuncs.h