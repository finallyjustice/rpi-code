#include "io.h"
#include "uart.h"

#define SYSTIMERCLO 0x20003004

// register the secure monitor vector
extern void init_secure_monitor();

// code in TrustZone Normal World
void normal_world()
{
	while(1)
	{
		int time = GET32(SYSTIMERCLO); // obtain current time counter
		cprintf("$$ Say hello in TrustZone Normal World!\n");
		cprintf("$$ Current Time in Normal World: %d 0x%x\n", time, time);
		// trnasit to Secure Monitor
		asm volatile("smc #0\n\t") ;
	}
}

int bootmain(void)
{
	uart_init();

	printstr("@@@@@ TrustZone Sample Program on Raspberry Pi B+ @@@@@\n");

	// register the TrustZone secure monitor vector
	init_secure_monitor(normal_world);
	printstr("@@@@@ Secure Monitor Vector Registration Successfully! @@@@@\n");
	cprintf("\n\n");

	int i;
	for(i=0; i<10; i++)
	{
		cprintf("## Say hello in TrustZone Secure World!\n");
		int time = GET32(SYSTIMERCLO); // obtain current time counter 
		// transit to Secure Monitor
		asm volatile("smc #0\n\t");
		// back from Secure Monitor
		cprintf("## Time in Secure World before Transition: %d 0x%x\n", time, time);
	};

	while(1);
}
