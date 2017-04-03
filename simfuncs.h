/* Simulation function header for Computer Systems Project 1
 * Author: Kaven Peng kavenp 696573
 *
 */

#ifndef __SIMFUNCS__
#define __SIMFUNCS__

#include <stdio.h>
#include <stdlib.h>
#include "memnode.h"
#include "process.h"

void simulateFCFS(Queue_t *processQ, memList_t *memory);
void simulateMulti(Queue_t *processQ, memList_t *memory);
void loadReadyProcess(Queue_t *processQ, Queue_t *readyQ, int counter);
void loadProcessIntoMem(process_t *process, memList_t *memory, int counter);
process_t *stepCPU(process_t *process, memList_t *memory, int *counterp, int *runtimep, int *runningp);
void printCPUStatus(process_t *currP, memList_t *memory, int counter);
void terminateProcess(process_t *currP, memList_t *memory);

#endif //simfuncs.h