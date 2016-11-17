
#ifndef READY_QUEUE_H
#define READY_QUEUE_H

struct _PCB {

	void *esp;	//this proccess's stack pointer
	struct _PCB *next;	//next process in the queue

};
typedef struct _PCB PCB;

extern PCB *CURRENT_PROCESS;

#define STACK_SIZE 		1024
#define NUM_PROCESSES 	10

//creates and allocates a PCB with its own stack running a particular function
PCB *createProcess(const void *const function);

//takes a PCB and places it on the ready queue
void ready_enqueue(PCB *const pcb);

//gets the top of the ready queue
PCB *ready_dequeue(void);

#endif
