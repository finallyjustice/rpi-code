#include "io.h"
#include "uart.h"

#define GPFSEL1 0x20200004
#define GPFSEL2 0x20200008
#define GPFSEL3 0x2020000C
#define GPFSEL4 0x20200010
#define GPSET0  0x2020001C
#define GPSET1  0x20200020
#define GPCLR0  0x20200028
#define GPCLR1  0x2020002C

// first "C" function
int bootmain(void)
{
	// init uart to enable debugging with TTL
    uart_init();

	cprintf("Demo of LED Light on Raspberry Pi B+\n");

	unsigned int ra;

	ra =  GET32(GPFSEL4);
	ra &= ~(7<<21);
	ra |= 1<<21;
	PUT32(GPFSEL4, ra);

	while(1)
	{
		// turn on the ACT LED light
		PUT32(GPSET1, 1<<(47-32));
		
		// wait for a while
		for(ra=0; ra<0x100000; ra++) 
			dummy(ra);

		// turn off the ACT LED light
		PUT32(GPCLR1, 1<<(47-32));
		
		for(ra=0; ra<0x100000; ra++) 
			dummy(ra);
	}

	while(1);    
	return(0);
}
