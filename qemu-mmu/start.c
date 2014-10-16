#include "uart.h"

volatile unsigned char * const UART0_BASE_TMP = (unsigned char *)0x0101f1000;

extern void kmain(void);

void print_string(const char *str)
{
	while(*str != '\0')
	{
		*UART0_BASE_TMP = *str;
		str++;

		if(*str == '\n')
		{
			*UART0_BASE_TMP = '\r';
		}
	}
}

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
	print_string("SWI Handler\n");
}

void bootmain()
{
	print_string("Hello World\n");
	asm volatile("swi #0x1");
	print_string("Back from SWI!\n");

	while(1);
	kmain();
}
