#include "uart.h"
#include "types.h"

static volatile uint *uart_base;

void kmain(void)
{
	cprintf("kmain!!!\n");

	while(1);
	return;
}
