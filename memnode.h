/* Memnode header for Computer Systems Project 1
 * Author: Kaven Peng kavenp 696573
 *
 */

#ifndef __MEMNODE__
#define __MEMNODE__
#include "process.h"

typedef struct memNode{
	int memSize;
	int empty;
	int timePlaced;
	process_t *process;
	struct memNode *prev;
	struct memNode *next;
} memNode_t;

typedef struct memList{
	int memTotalSize;
	memNode_t *head;
	memNode_t *foot;
} memList_t;

#endif //memnode.h