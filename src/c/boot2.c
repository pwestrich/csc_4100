
#include "definitions.h"
#include "ready_queue.h"
#include "interrupts.h"

//from boot2.S
void clearScr(void);
void writeScr(const char *const message, const int row, const int col);
void setupPIC();
void initTimer(const uint32_t ms);
void go();

int main(void) __attribute__((noreturn));
void p0(void) __attribute__((noreturn));;
void p1(void) __attribute__((noreturn));;
void p2(void) __attribute__((noreturn));;
void p3(void) __attribute__((noreturn));;
void p4(void) __attribute__((noreturn));;
void p5(void) __attribute__((noreturn));;
void p6(void) __attribute__((noreturn));;
void p7(void) __attribute__((noreturn));;
void p8(void) __attribute__((noreturn));;
void p9(void) __attribute__((noreturn));;

//entry point of stage 2 of the operating system
int main(void){

  //make sure interrupts are off
  __asm__("cli");

  //clear the screen first
  clearScr();

  //set up the IDT
  initInterrupts();

  setupPIC();
  initTimer(50);

  //create and enqueue each process
  PCB *pcb = createProcess(p0);
  ready_enqueue(pcb);

  pcb = createProcess(p1);
  ready_enqueue(pcb);

  pcb = createProcess(p2);
  ready_enqueue(pcb);

  pcb = createProcess(p3);
  ready_enqueue(pcb);

  pcb = createProcess(p4);
  ready_enqueue(pcb);

  pcb = createProcess(p5);
  ready_enqueue(pcb);

  pcb = createProcess(p6);
  ready_enqueue(pcb);

  pcb = createProcess(p7);
  ready_enqueue(pcb);

  pcb = createProcess(p8);
  ready_enqueue(pcb);

  pcb = createProcess(p9);
  ready_enqueue(pcb);

  writeScr("Preparing to start processes...", 0, 0);

  //start first process
  go();

	//should never get here
	panic("Reached end of main().");

}

bool assert(const bool condition, const char *const message){

  if (!condition){

    panic(message);

  }

}

void panic(const char *const message){

  //ignore interrupts from here on out
  __asm__("cli");

  //clear screen
  clearScr();

  //write message
  writeScr("Panic: ", 0, 0);
  writeScr(message, 1, 0);

  //halt
  while (true);

}

void p0(void){

  int i = 0;
  char *const message = "process p0:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 1, 0);

    i = (i + 1) % 10;

  }

}

void p1(void){

  int i = 0;
  char *const message = "process p1:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 2, 0);

    i = (i + 1) % 10;

  }

}

void p2(void){

  int i = 0;
  char *const message = "process p2:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 3, 0);

    i = (i + 1) % 10;

  }

}

void p3(void){

  int i = 0;
  char *const message = "process p3:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 4, 0);

    i = (i + 1) % 10;

  }

}

void p4(void){

  int i = 0;
  char *const message = "process p4:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 5, 0);

    i = (i + 1) % 10;

  }

}

void p5(void){

  int i = 0;
  char *const message = "process p5:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 6, 0);

    i = (i + 1) % 10;

  }

}

void p6(void){

  int i = 0;
  char *const message = "process p6:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 7, 0);

    i = (i + 1) % 10;

  }

}

void p7(void){

  int i = 0;
  char *const message = "process p7:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 8, 0);

    i = (i + 1) % 10;

  }

}

void p8(void){

  int i = 0;
  char *const message = "process p8:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 9, 0);

    i = (i + 1) % 10;

  }

}

void p9(void){

  int i = 0;
  char *const message = "process p2:  ";

  while (true){

    message[12] = ((char)i + '0');
    writeScr(message, 10, 0);

    i = (i + 1) % 10;

  }

}
