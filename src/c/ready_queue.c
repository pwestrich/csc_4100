
#include "definitions.h"
#include "ready_queue.h"

static PCB *ready_queue_front 	= NULL;
static PCB *ready_queue_back	= NULL;
	   PCB *CURRENT_PROCESS	 	= NULL;

static PCB PCBS[NUM_PROCESSES]; //make room for PCBs

static int STACKS[NUM_PROCESSES][STACK_SIZE];

PCB *createProcess(const void *const function){

	static int index = 0;

	assert(index < NUM_PROCESSES, "Made too many processes.");

	PCB *pcb = &PCBS[index];

	int *const stack = STACKS[index++];
	int *const stackTop = stack + STACK_SIZE;

	//set up this process's stack
	*(stackTop - 0)  = 0x200;	//EFLAGS with interrupts on
	*(stackTop - 1)  = CS;		//CS
	*(stackTop - 2)  = (int)function;//EIP
	*(stackTop - 3)  = 0;		//EBP
	*(stackTop - 4)  = 0;		//ESP
	*(stackTop - 5)  = 0;		//EDI
	*(stackTop - 6)  = 0;		//ESI
	*(stackTop - 7)  = 0;		//EDX
	*(stackTop - 8)  = 0;		//ECX
	*(stackTop - 9)  = 0;		//EBX
	*(stackTop - 10) = 0;		//EAX
	*(stackTop - 11) = DS;		//DS
	*(stackTop - 12) = DS;		//ES
	*(stackTop - 13) = DS;		//FS
	*(stackTop - 14) = DS;		//GS

	//set the PCB's stack top to the end of this block
	pcb->esp = (void*)(stackTop - 14); 

	assert(pcb->esp != NULL, "Null stack pointer when creating process.");

	return pcb;

}


void ready_enqueue(PCB *const pcb){

	//case 1: nothing on queue
	if (ready_queue_front == NULL){

		//this item becomes both front and back
		ready_queue_front = pcb;
		ready_queue_back  = pcb;

	} else {

		//this item becomes the back
		ready_queue_back->next = pcb;
		ready_queue_back = pcb;

	}

	//this it the end of the queue either way
	pcb->next = NULL;

}

PCB *ready_dequeue(void){

	//get the front of the queue
	PCB *const returnThis = ready_queue_front;

	//if we got a NULL, we're in trouble
	assert(returnThis != NULL, "Front of ready queue was null.");

	//next item is now front
	ready_queue_front = ready_queue_front->next;
	returnThis->next = NULL;

	assert(returnThis->esp != NULL, "Null stack pointer when dequeueing process.");

	return returnThis;

}
