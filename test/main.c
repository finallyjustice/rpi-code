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

	int s = 100;
	int arr[100];
	int i;
	for(i=0; i<s; i++)
	{
		arr[i] = 0x100;
	}

	hexstring(arr[10]);
	uprint("Test TrustZone\n");
	uprint("Hello World\n");
	
	while(1);
}
