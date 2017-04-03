/* Main function for Computer Systems Project 1
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

/* Define constants */

#define LINE_LEN 50
#define PVALNUM 4

/* Extern declarations */

extern int optind;
extern char *optarg;

/*function declarations */
process_t *createProcess(int *pVals);

int main(int argc, char** argv)
{
	char flag;
	char *schedule, *token; 
	char inputLine[LINE_LEN];
	FILE *fp;
	int Msize, count;
	int pVals[PVALNUM];
	process_t *newP;
	Queue_t *processQ;
	memList_t *memory;

	/* read input flags and set options */
	while((flag = getopt(argc,argv,"f:a:m:")) != EOF)
	{
		switch(flag)
		{
			case 'f':
				fp = fopen(optarg,"r");
				if(fp == NULL) {
					perror ("Unable to open file.");
					exit(1);
				}
				break;
			
			case 'a':
				//set schedule method to fcfs or multi
				if(strcmp(optarg, "fcfs") == 0) {
					schedule = optarg;
				} else if(strcmp(optarg, "multi") == 0) {
					schedule = optarg;
				} else {
					//unknown option
					fprintf(stderr, "Unknown method %s\n", optarg);
				}
				break;

			case 'm':
				//set memsize
				Msize = atoi(optarg);
				break;
		}
	}
	//create a queue to place processes entered 
	processQ = createQueue();
	//create  and initialize memory
	memory = createMemory();
	initMemory(memory, Msize);
	//while not EOF
	while(fgets(inputLine, LINE_LEN, fp) != NULL) 
	{
		count = 0;
		//printf("input: %s\n", inputLine);
		//first token
		token = strtok(inputLine," ");
		while(token != NULL) {
			pVals[count] = atoi(token);
			count++;
			token = strtok(NULL," ");
		}
		newP = createProcess(pVals);
		enQueue(processQ, newP);
	}
	if(strcmp(schedule, "fcfs") == 0) {
		//run fcfs
		simulateFCFS(processQ, memory);
	}else {
		//run multi
		simulateMulti(processQ, memory);
	}
	//free everything
	freeQ(processQ);
	freeMemory(memory);
	fclose(fp);
	return 0;
}

process_t
*createProcess(int *pVals) {
	process_t *newP = (process_t*)malloc(sizeof(process_t));
	newP->timeCreate = pVals[0];
	newP->processId = pVals[1];
	newP->size = pVals[2];
	newP->runtime = pVals[3];
	return newP;
}



