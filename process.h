/* Process type header for Computer Systems Project 1
 * Author: Kaven Peng kavenp 696573
 *
 */

#ifndef __PROCESS__
#define __PROCESS__

typedef struct {
	int quantum;
	int active;
	int timeCreate;
	int processId;
	int size;
	int runtime;
} process_t;

#endif //process.h