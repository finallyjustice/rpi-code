#include "uart.h"

void Normal_World()
{
	while(1)
	{
		cprintf("Hello from new Normal world\n");
		asm volatile("smc #0\n\t");
	}
}
