#include "io.h"
#include "uart.h"

extern void monitorInit();
extern void normal_start();

void sec_uprint(char *s)
{
	int i = 0;
	while(s[i])
	{
		if(s[i] == '\n')
			sec_uart_send('\r');
		sec_uart_send(s[i]);
		i++;
	}
}

void secure_handler(void)
{
	while(1)
	{
		//sec_cprintf("Hello, this is secure handler!\n");
		sec_uprint("Hello, this is secure handler!\n");
		asm volatile("smc #0\n\t");
	}
}

void bootmain(void)
{
	uart_init();
	
	sec_uprint("Test TrustZone!\n");

	monitorInit(normal_start);
		
	secure_handler();
}
