
#include "interrupts.h"

//definition for an entry in the IDT
struct _IDTEntry {

	uint16_t	base_low;	//low half of function address
	uint16_t	selector;	//code selector for function
	uint8_t		alwaysZero;	//0
	uint8_t		access;		//access bits
	uint16_t	base_high;	//high half of function address

} __attribute__((packed));
typedef struct _IDTEntry IDTEntry;

struct _LIDT {

	uint16_t limit;	//sizeof idt
	void	 *base;	//address of idt

} __attribute__((packed));
typedef struct _LIDT LIDT;

extern void loadIDT(LIDT *const lidt);
void setInterrupt(void *const function, const uint8_t index, const uint8_t access, const uint16_t selector);
void defaultInterrupt(void) __attribute((noreturn));

//the actual IDT itself
static IDTEntry idt[256];
static LIDT lidtr;

void initInterrupts(void){

	//set the first 32 interrupts to call panic
	for (uint8_t i = 0; i < 32; ++i){

		setInterrupt(defaultInterrupt, i, 0x8E, CS);

	}

	//set number 32 to be our schedule function
	setInterrupt(schedule, 32, 0x8E, CS);

	//set the remaining interrupts to be the default handler too
	for (uint8_t i = 33; i <= 250; ++i){

		setInterrupt(defaultInterrupt, i, 0x8E, CS);

	}

	//now tell the processor to use our IDT
	lidtr.limit = (sizeof(IDTEntry) * 256) - 1;
	lidtr.base = idt;

	loadIDT(&lidtr);

}

//sets the given function to the interrupt number
void setInterrupt(void *const function, const uint8_t index, const uint8_t access, const uint16_t selector){

	//these are easy
	idt[index].access 		= access;
	idt[index].selector 	= selector;
	idt[index].alwaysZero 	= 0;

	//hope this works
	idt[index].base_low		= ((uint16_t)function) & 0xFFFF;
	idt[index].base_high	= ((uint32_t)function >> 16) & 0xFFFF;

}

void defaultInterrupt(void){

	panic("Invalid interrupt called.");

}
