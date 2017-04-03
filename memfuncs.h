/* Memory function header for Computer Systems Project 1
 * Author: Kaven Peng kavenp 696573
 *
 */

#ifndef __MEMFUNCS__
#define __MEMFUNCS__

#include <stdio.h>
#include <stdlib.h>
#include "memnode.h"
#include "process.h"

memNode_t *newNode(int memSize, int empty, int timePlaced, process_t *process);
memList_t *createMemory();
void initMemory(memList_t *memory, int memSize);
int insertProcess(memList_t *memory, process_t *process, int time);
void mergeHoles(memList_t *memory);
int clearMemory(memList_t *memory);
void removeProcessNode(memList_t *memory, memNode_t *node);
void printMemory(memList_t *memory);
void freeMemory(memList_t *memory);
int calcNumProcesses(memList_t *memory);
int calcNumHoles(memList_t *memory);
int calcMemUsage(memList_t *memory);

#endif //memfuncs.h