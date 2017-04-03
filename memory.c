/* Memory code for Computer Systems Project 1
 * Author: Kaven Peng kavenp 696573
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "memnode.h"
#include "process.h"
#include "memfuncs.h"

/* timePlaced is -1 for all empty memory blocks */
memNode_t
*newMemNode(int memSize, int empty, int timePlaced, process_t *process) 
{
	memNode_t *new = (memNode_t*)malloc(sizeof(memNode_t));
	if(new == NULL) {
		fprintf(stderr, "Error during malloc.\n");
		exit(1);
	}
	new->memSize = memSize;
	new->empty = empty;
	new->timePlaced = timePlaced;
	new->process = process;
	new->prev = NULL;
	new->next = NULL;
	return new;
}

memList_t
*createMemory() 
{
	memList_t *list;
	list = (memList_t*)malloc(sizeof(memList_t));
	if(list == NULL) {
		fprintf(stderr, "Error during malloc.\n");
		exit(1);
	}
	list->head = NULL;
	list->foot = NULL;
	return list;
}

void
initMemory(memList_t *memory, int memSize)
{
	memNode_t *init;
	init = newMemNode(memSize, 1, -1, NULL);
	memory->memTotalSize = memSize;
	memory->head = init;
	memory->foot = init;
}

/* Returns 1 if no space to insert */
int
insertProcess(memList_t *memory, process_t *process, int time) 
{
	memNode_t *new = newMemNode(process->size, 0, time, process);
	//start searching from highest address block
	//which is foot here
	memNode_t *curr = memory->foot;
	memNode_t *nextNode, *prevNode;
	while(curr != NULL) {
		nextNode = curr->next;
		prevNode = curr->prev;
		if(curr->empty && (curr->memSize > process->size)) {
			//if there is already a process occupying next block
			//we insert the new process between curr and next block
			if(nextNode != NULL) {
				curr->next = new;
				new->prev = curr;
				new->next = nextNode;
				nextNode->prev = new;
			}else{
				//foot block has space
				curr->next = new;
				new->prev = curr;
				memory->foot = new;
			}
			//finished insertion, now need to alter memsize of block
			curr->memSize -= new->memSize;
			return 0;
		} else if(curr->empty && (curr->memSize == process->size)) {
			//new process block replaces current block
			if(nextNode != NULL && prevNode != NULL) {
				new->next = nextNode;
				new->prev = prevNode;
				nextNode->prev = new;
				prevNode->next = new;
				free(curr);
				return 0;
			}else if (nextNode == NULL && prevNode == NULL) {
				//curr block is init empty memory block
				memory->head = new;
				memory->foot = new;
				free(curr);
				return 0;
			}else if (nextNode == NULL && prevNode != NULL) {
				//curr is foot
				memory->foot = new;
				new->prev = prevNode;
				prevNode->next = new;
				free(curr);
				return 0;
			}else{
				//curr is head
				memory->head = new;
				new->next = nextNode;
				nextNode->prev = new;
				free(curr);
				return 0;
			}

		} else {
			//move to next block
			curr = curr->prev;
		}
	}
	//if reaches here, means there is no space available
	//so set value to clear memory
	return 1;
}

void
mergeHoles(memList_t *memory)
{
	memNode_t *curr = memory->head;
	memNode_t *nextNode;
	while(curr != NULL) {
		nextNode = curr->next;
		if(nextNode == NULL) {
			//at foot, finished merging
			break;
		}
		if(curr->empty && nextNode->empty) {
			//holes adjacent so merge
			curr->next = nextNode->next;
			if(nextNode != memory->foot) {
				nextNode->next->prev = curr;
			}else{
				memory->foot = curr;
			}
			curr->memSize += nextNode->memSize;
			free(nextNode);
		}else{
			//move onto next block
			curr = curr->next;
		}
	}
}

/* Returns 1 if fails to clear any memory */
int
clearMemory(memList_t *memory) 
{
	memNode_t *curr = memory->head;
	memNode_t *remNode = NULL;
	//first run through to initialize a node to be removed
	while(curr != NULL) {
		if((!curr->empty) && (!curr->process->active)) {
			remNode = curr;
			break;
		}else{
			curr = curr->next;
		}
	}
	if(remNode == NULL) {
		return 1;
		//no nodes to clear
	}
	//reset curr value
	curr = memory->head;
	//second run through to compare nodes to remove
	while(curr != NULL) {
		if((!curr->empty) && (curr->process->runtime == 0)) {
			if(curr->memSize > remNode->memSize) {
				remNode = curr;
				curr = curr->next;
			}else if (curr->memSize == remNode->memSize) {
				if(curr->timePlaced < remNode->timePlaced) {
					remNode = curr;
				}
				curr = curr->next;
			}
		}else{
			curr = curr->next;
		}
	}
	//now we have the correct remNode, just remove it
	removeProcessNode(memory, remNode);
	mergeHoles(memory);
	return 0;
}

void
removeProcessNode(memList_t *memory, memNode_t *node) 
{
	memNode_t *newMem = newMemNode(node->memSize, 1, -1, NULL);
	memNode_t *prevNode = node->prev;
	memNode_t *nextNode = node->next;
	if(prevNode == NULL && nextNode == NULL) {
		//only one block of memory
		memory->head = newMem;
		memory->foot = newMem;
		free(node);
	} else if (nextNode == NULL) {
		//foot
		memory->foot = newMem;
		prevNode->next = newMem;
		newMem->prev = prevNode;
		free(node);
		return;
	} else if (prevNode == NULL) {
		//head
		memory->head = newMem;
		nextNode->prev = newMem;
		newMem->next = nextNode;
		free(node);
		return;
	} else {
		//in the middle
		newMem->next = nextNode;
		newMem->prev = prevNode;
		nextNode->prev = newMem;
		prevNode->next = newMem;
		free(node);
		return;
	}
}

void
printMemory(memList_t *memory) 
{
	memNode_t *curr = memory->head;
	while(curr != NULL) {
		printf("Memory block| size = %d | empty = %d | time = %d\n", curr->memSize, 
			curr->empty, curr->timePlaced);
		if(curr->process != NULL) {
			printf("processId = %d\n", curr->process->processId);
		}else{
			printf("processId = NULL\n");
		}
		curr = curr->next;
	}
}

void
freeMemory(memList_t *memory) 
{
	memNode_t *curr, *next;
	if(memory != NULL) {
		curr = memory->head;
		while(curr != NULL) {
			next = curr->next;
			free(curr);
			curr = next;
		}
	}
}

int
calcNumProcesses(memList_t *memory)
{
	int count = 0;
	memNode_t *curr = memory->head;
	while(curr != NULL) {
		if(!curr->empty) {
			count++;
		}
		curr = curr->next;
	}
	return count;
}

int 
calcNumHoles(memList_t *memory)
{
	int count = 0;
	memNode_t *curr = memory->head;
	while(curr != NULL) {
		if(curr->empty) {
			count++;
		}
		curr = curr->next;
	}
	return count;
}

int 
calcMemUsage(memList_t *memory)
{
	int processTotal = 0;
	int memTotal = memory->memTotalSize;
	double percentage;
	memNode_t *curr = memory->head;
	while(curr != NULL) {
		if(!curr->empty) {
			processTotal += curr->memSize;
		}
		curr = curr->next;
	}
	percentage = ((double)processTotal/memTotal)* 100;
	percentage = ceil(percentage);	
	return percentage;
}