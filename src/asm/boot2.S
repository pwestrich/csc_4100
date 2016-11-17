
.intel_syntax noprefix

/* public funcitons */
.global clearScr
.global writeScr
.global initTimer
.global setupPIC
.global loadIDT
.global schedule
.global go

/* external symbols */
.extern ready_enqueue
.extern ready_dequeue
.extern CURRENT_PROCESS

/* constants */
.set SCREEN_START,		0xB8000
.set SCREEN_WIDTH, 		80
.set SCREEN_HEIGHT, 	25
.set SCREEN_AREA,		80 * 25
.set SCREEN_AREA_COLOR,	80 * 25 * 2
.set SCREEN_TEXT_COLOR,	0x07
.set QUANTUM,			10

/* variables */
scheduleTimer:
	.word QUANTUM

/* void clearScr(void); */
clearScr:
	
	/* establish stack frame */
	push	ebp
	mov 	ebp, esp

	/* push used registers */
	push 	eax
	push 	ebx	
	push 	ecx

	xor 	eax, eax
	mov 	ebx, SCREEN_START
	mov 	ecx, ebx
	add 	ecx, SCREEN_AREA

	clearScr_loop:
	mov 	[ebx], eax
	add 	ebx, 4
	cmp 	ebx, ecx
	jne 	clearScr_loop

	/* pop used registers */
	pop 	ecx
	pop 	ebx
	pop 	eax

	/* get out of here */
	mov 	esp, ebp
	pop 	ebp
	ret

/* void writeScr(char *string, int row, int col); */
writeScr:

	/* establish stack frame */
	push	ebp
	mov 	ebp, esp

	/* push used registers */
	push 	eax
	push 	ebx
	push 	ecx

	/* calcualte start of printing */
	mov 	ecx, [ebp + 12]
	imul	ecx, SCREEN_WIDTH
	mov 	eax, [ebp + 16]
	add		ecx, eax
	shl		ecx, 1
	add 	ecx, SCREEN_START

	/* start printing */
	mov 	ebx, [ebp + 8]
	mov 	ah, SCREEN_TEXT_COLOR

	writescr_print_start:

	mov 	al, [ebx]
	cmp		al, 0
	jz		writescr_print_done

	mov 	[ecx], ax
	inc 	ebx
	add 	ecx, 2
	jmp writescr_print_start

	writescr_print_done:
	/* pop used registers */
	pop 	ecx
	pop 	ebx
	pop 	eax

	/* get out of here */
	mov 	esp, ebp
	pop 	ebp
	ret

setupPIC:
	
	 /* establish stack frame */
	push	ebp
	mov 	ebp, esp

	/* save registers*/
	push 	eax 

	mov 	al, 0x11
	out 	0x20, al

	mov 	al, 0x11
 	out 	0xA0, al

 	mov 	al, 0x20
 	out 	0x21, al

 	mov 	al, 0x28
 	out 	0xA1, al

 	mov 	al, 0x04
 	out 	0x21, al

 	mov 	al, 0x02
 	out 	0xA1, al

 	mov 	al, 0x01
 	out 	0x21, al

 	mov 	al, 0x01
 	out 	0xA1, al

 	mov 	al, 0x00
 	out 	0x21, al

 	mov 	al, 0x00
 	out 	0xA1, al

 	mov 	al, 0xfe
 	out 	0x21, al

 	mov 	al, 0xff
 	out 	0xa1, al

 	/* restore regitsers */
	pop 	eax

	/* get out of here */
	mov 	esp, ebp
	pop 	ebp
	ret

/* void initTimer(uint32_t ms); */
 initTimer:

 	/* establish stack frame */
	push	ebp
	mov 	ebp, esp

	/* save registers*/
	push 	eax
	push 	edx

	/* move ms into register and calculate the number to send to PIC*/
	mov 	eax, [ebp + 8]
	mov 	edx, 1193
	mul		edx
	mov 	edx, eax

	mov 	al,	0b00110110
	out 	0x43, al

	mov 	eax, edx
	out 	0x40, al
	xchg 	al, ah
	out 	0x40, al

	/* set timer count to QUANTUM */
	mov 	eax, QUANTUM
	mov 	scheduleTimer, eax 

	/* restore regitsers */
	pop 	edx
	pop 	eax

	/* get out of here */
	mov 	esp, ebp
	pop 	ebp
	ret

loadIDT:

	/* establish stack frame */
	push	ebp
	mov 	ebp, esp

	push 	eax

	/* move argument into eax*/
	mov 	eax, [ebp + 8]
	lidt 	[eax]

	pop 	eax

	/* get out of here */
	mov 	esp, ebp
	pop 	ebp
	ret

schedule:
	
	/* save the state of all the registers in this process */
	pushad
	push 	ds
	push 	es
	push 	fs
	push 	gs

	/* decrement the number of ticks, switch processes if zero */
	mov 	eax, scheduleTimer
	dec		eax
	mov 	scheduleTimer, eax
	jnz		schedule_done

	/* save ESP to this PCB's block */
	mov 	eax, CURRENT_PROCESS
	mov 	[eax], esp

	/* enqueue this PCB to the ready queue */
	push 	eax
	call 	ready_enqueue
	add 	esp, 4

	/* dequeue next PCB and restore its state */
	call 	ready_dequeue

	/* set this as the current process and reset timer */
	mov 	CURRENT_PROCESS, eax
	mov 	esp, [eax]
	mov 	eax, QUANTUM
	mov 	scheduleTimer, eax

	schedule_done:

	mov 	al, 0x20
	out		0x20, al 

	/* restore process state and continue */
	pop 	gs
	pop 	fs
	pop 	es
	pop 	ds
	popad
	iret

go:

	call ready_dequeue

	/* set this as the current process */
	mov CURRENT_PROCESS, eax

	/*restore stack pointer */
	mov esp, [eax]

	/* restore process state */
	pop gs
	pop fs
	pop es
	pop ds
	popad

	/* go */
	iret
