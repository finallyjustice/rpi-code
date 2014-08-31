#include "io.h"
#include "uart.h"

void uprint(char *s)
{
	int i = 0;
	while(s[i])
	{
		uart_send(s[i]);
		i++;
	}
	uart_send('\r');
	uart_send('\n');
}

// cache the input from user via UART
char buf[64];

// first "C" function
int bootmain(void)
{
	// init uart to enable debugging with TTL
    uart_init();

	// print string
	uprint("This is the echo service:");

	// print char
	uart_send('$');

	char c;
	int i = 0;
	while(1)
	{
		c = uart_recv();
		uart_send(c);

		if(c == '\r')
		{
			buf[i] = '\0';
			i = 0;

			uart_send('\r');
			uart_send('\n');
			uart_send('#');
			uprint(buf);

			uart_send('$');
		}
		else
		{
			buf[i] = c;
			i++;
		}
	}

	while(1);
    
	return(0);
}
