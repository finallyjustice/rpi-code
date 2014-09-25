#include "uart.h"

void copy_vectors(void) 
{
	extern unsigned int vector_start;
	extern unsigned int vector_end;
	unsigned int *vectors_src = &vector_start;
	unsigned int *vectors_dst = (unsigned int *)0;
 
	while(vectors_src < &vector_end)
	{
		*vectors_dst++ = *vectors_src++;
	}
}

void swi_handler(unsigned int arg)
{
	cprintf("SWI: %x\n", arg);
}

void bootmain()
{
	cprintf("Hello World %d\n", 333);
	asm volatile("swi #0x1");
	cprintf("Back from SWI!\n");
}
