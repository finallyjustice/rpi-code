#include "io.h"
#include "uart.h"

void uprint(char *s)
{
	int i = 0;
	while(s[i])
	{
		if(s[i] == '\n')
			uart_send('\r');
		
		uart_send(s[i]);
		i++;
	}
}

// cache the input from user via UART
char buf[64];

// first "C" function
int bootmain(void)
{
	// init uart to enable debugging with TTL
    uart_init();

	// print string
	uprint("This is the echo service:\n");

						
	
	while(1);
    
	return(0);
}
