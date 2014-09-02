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

int bootmain(void)
{
	uart_init();
	
	uprint("Test TrustZone\n");
	//int value;
	int addr = &uprint;
	hexstring(addr);
	while(1);
}
