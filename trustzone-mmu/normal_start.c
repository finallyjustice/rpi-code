#include "uart.h"

void Normal_World()
{
	int i;
	for(i=0; i<10; i++)
	{
		sec_cprintf("Hello from new Normal world\n");
		asm volatile("smc #0\n\t");
	}

	sec_cprintf("Pending in normal world\n");
	while(1);
}
