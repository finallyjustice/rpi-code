#include "io.h"
#include "uart.h"

extern void monitorInit();
extern void normal_start();

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
	
	uprint("Test TrustZone!\n");

	monitorInit(normal_start);
	
	uprint("Install Monitor Successfully\n");

	int i;
	for(i=0; i<10; i++)	
	{
		cprintf("hello from Secure world\n");
		asm volatile("smc #0\n\t");
	};

	while(1);
}
