
#ifndef DEFINITIONS_H
#define DEFINITIONS_H
//This file contains definitions for various things. They should be useful.

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

//just in case NULL isn't either
#ifndef NULL
#define NULL 0
#endif

//code and stack selectors
#define CS 	16
#define DS 	8

//why does this not exist?
typedef int bool;

//unsigned integer types
typedef unsigned char 	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;

//signed integer types
typedef char	int8_t;
typedef short	int16_t;
typedef int 	int32_t;

//important functions
bool assert(const bool condition, const char *const message);
void panic(const char *const message) __attribute__((noreturn));

//from boot2.S
extern void schedule(void);

#endif
