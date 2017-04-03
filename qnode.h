/* Queue node header for Computer Systems Project 1
 * Author: Kaven Peng kavenp 696573
 *
 */

#ifndef __QNODE__
#define __QNODE__
#include "process.h"

typedef struct QNode {
	process_t* process;
	struct QNode *next;
} QNode_t;

typedef struct Queue {
	QNode_t *front, *rear;
} Queue_t;

#endif //qnode.h