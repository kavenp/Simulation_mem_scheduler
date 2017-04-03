/* Simulation code for Computer Systems Project 1
 * Author: Kaven Peng kavenp 696573
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "process.h"
#include "qnode.h"
#include "qfuncs.h"
#include "memnode.h"
#include "memfuncs.h"
#include "simfuncs.h"

//define quantum constants
#define quantum1 2
#define quantum2 4
#define quantum3 8

void
simulateFCFS(Queue_t *processQ, memList_t *memory) 
{
	int counter = 0;
	int runtime = 0;
	int running = 0;
	//create new ready process queue
	Queue_t *readyQ = createQueue();
	QNode_t *temp;
	process_t *currentP;
	while(1) {
		//load any ready process from processQ to readyQ
		loadReadyProcess(processQ, readyQ, counter);
		//no processes currently running
		if(!running) {
			temp = deQueue(readyQ);
			if(temp != NULL) {
				//set running flag
				running = 1;
				//keeps track of currently running process and runtime left
				currentP = temp->process;
				currentP->active = 1;
				runtime = currentP->runtime;
				loadProcessIntoMem(currentP,memory,counter);
				printCPUStatus(currentP, memory, counter);
				currentP = stepCPU(currentP, memory, &counter, &runtime, &running);
				free(temp);
			} else {
				//no ready processes
				counter++;
			}
		} else {
			//already running a process
			currentP = stepCPU(currentP, memory, &counter, &runtime, &running);
		}
		//check for simulation end
		if(isEmptyQ(processQ) && isEmptyQ(readyQ) && running == 0) {
			//simulation is over
			printf("time %d, simulation finished.\n", counter);
			freeQ(readyQ);
			break;
		}
	}
}

void
simulateMulti(Queue_t *processQ, memList_t *memory)
{
	int counter = 0;
	int runtime = 0;
	int running = 0;
	//create 3 level of queues
	Queue_t *Q1 = createQueue();
	Queue_t *Q2 = createQueue();
	Queue_t *Q3 = createQueue();
	QNode_t *temp;
	process_t *currentP = NULL;
	while(1) {
		//load any ready process into Q1
		loadReadyProcess(processQ, Q1, counter);
		if(!running) {
			if(currentP != NULL) {
				//previously running process has not terminated
				if(currentP->quantum == quantum1) {
					enQueue(Q2, currentP);
				}else {
					enQueue(Q3, currentP);
				}
			}
			if(!isEmptyQ(Q1)) {
				running = 1;
				temp = deQueue(Q1);
				currentP = temp->process;
				//set quantum values
				runtime = quantum1;
				currentP->quantum = quantum1;
				currentP->active = 1;
				loadProcessIntoMem(currentP, memory, counter);
				printCPUStatus(currentP, memory, counter);
				currentP = stepCPU(currentP, memory, &counter, &runtime, &running);
				free(temp);
			} else if(!isEmptyQ(Q2)) {
				running = 1;
				temp = deQueue(Q2);
				currentP = temp->process;
				runtime = quantum2;
				currentP->quantum = quantum2;
				currentP->active = 1;
				loadProcessIntoMem(currentP, memory, counter);
				printCPUStatus(currentP, memory, counter);
				currentP = stepCPU(currentP, memory, &counter, &runtime, &running);
				free(temp);
			} else if(!isEmptyQ(Q3)) {
				running = 1;
				temp = deQueue(Q3);
				currentP = temp->process;
				runtime = quantum3;
				currentP->quantum = quantum3;
				currentP->active = 1;
				loadProcessIntoMem(currentP, memory, counter);
				printCPUStatus(currentP, memory, counter);
				currentP = stepCPU(currentP, memory, &counter, &runtime, &running);
				free(temp);
			} else if(!isEmptyQ(processQ)){
				//no processes ready
				running = 0;
				counter++;
			} else {
				//all queues empty
				if(currentP == NULL) {
					//last process finished
					printf("time %d, simulation finished.\n", counter);
					freeQ(Q1);
					freeQ(Q2);
					freeQ(Q3);
					break;
				} else {
					//still running last process
					currentP = stepCPU(currentP, memory, &counter, &runtime, &running);
				}
			}
		} else {
			//already running process
			currentP = stepCPU(currentP, memory, &counter, &runtime, &running);
		}
	}
}

void
loadReadyProcess(Queue_t *processQ, Queue_t *readyQ, int counter)
{
	QNode_t *curr = processQ->front;
	QNode_t *temp;
	while(curr != NULL) {
		if(curr->process->timeCreate == counter) {
			enQueue(readyQ, curr->process);
			//after adding to queue, possible same time exist in next node
			curr = curr->next;
			//remove node and free
			temp = deQueue(processQ);
			free(temp);
		} else {
			//won't be any times after this that will match
			return;
		}
	}
}

void
loadProcessIntoMem(process_t *process, memList_t *memory, int counter)
{
	memNode_t *curr = memory->head;
	while(curr != NULL) {
		if(!curr->empty && process->processId == curr->process->processId) {
			//case where process is already in memory
			return;
		}
		curr = curr->next;
	}
	//stays in loop if no space in memory
	while(insertProcess(memory, process, counter)) {
		//clears memory until there is enough space
		clearMemory(memory);
	}
	//process loaded
}

void
terminateProcess(process_t *currP, memList_t *memory)
{
	memNode_t *curr = memory->head;
	while(curr != NULL) {
		//short circuit eval making certain that process won't be null
		if(!curr->empty && (curr->process->processId == currP->processId)) {
			removeProcessNode(memory, curr);
			mergeHoles(memory);
		}
		curr = curr->next;
	}
} 

process_t
*stepCPU(process_t *process, memList_t *memory, int *counterp, int *runtimep, int *runningp)
{
	(process->runtime) -= 1;
	*counterp += 1;
	*runtimep -= 1;
	//reach end of quantum
	if(*runtimep == 0) {
		*runningp = 0;
		process->active = 0;
	}
	//process terminates
	if(process->runtime <= 0) {
		*runningp = 0;
		process->active = 0;
		terminateProcess(process, memory);
		return NULL;
	}
	return process;
}

void
printCPUStatus(process_t *currP, memList_t *memory, int counter)
{
	int currPID = currP->processId;
	int numProc, numHole, memUsage;
	numProc = calcNumProcesses(memory);
	numHole = calcNumHoles(memory);
	memUsage = calcMemUsage(memory);
	printf("time %d, %d running, numprocesses=%d, numholes=%d, memusage=%d%%\n",
		   counter, currPID, numProc, numHole, memUsage);
}