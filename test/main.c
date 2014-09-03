#include "io.h"
#include "uart.h"

extern void monitorInit();

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

void Normal_World()
{
	while(1) 
	{
		uprint("hello from Normal world\n");
		asm volatile("smc #0\n\t") ;
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

	monitorInit(Normal_World);
	
	uprint("Install Monitor Successfully\n");

	while(1) 
	{
		uprint("hello from Secure world\n\r");
		asm volatile("smc #0\n\t") ;
	};

	while(1);
}
