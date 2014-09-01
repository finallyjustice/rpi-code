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

void smc_handler(void)
{
	uprint("In SMC Handler");
	while(1);
}

// first "C" function
int bootmain(void)
{
	// init uart to enable debugging with TTL
    uart_init();

	// print string
	uprint("Test TrustZone");

	setup_monitor();
	uprint("Setup Secure Monitor!");

	int scr_val = 1;

	__asm__ volatile(
		"mrc p15, 0, r0, c1, c1, 0\n\t"
		"mov %0, r0\n\t"
		: "=r"(scr_val)
		: 
		: "r0"
	);

	if(scr_val & 0x1)
		uprint("TrustZone NS Bit is 1");
	else
		uprint("TrustZone NS Bit is 0");

	uprint("Begin Set TrustZone NS Bit to 1");
	
	__asm__ volatile(
		"mrc p15, 0, r0, c1, c1, 0\n\t"
		"mov r1, r0\n\t"
		"add r0, r1, #1\n\t"
		"mcr p15, 0, r0, c1, c1, 0\n\t"
		: 
		: 
		: "r0","r1"
	);

	uprint("End Set TrustZone NS Bit to 1");
	
	__asm__ volatile("smc #0");
	
	uprint("After SMC");

	/*scr_val = 0;

	__asm__ volatile(
		"mrc p15, 0, r0, c1, c1, 0\n\t"
		"mov %0, r0\n\t"
		: "=r"(scr_val)
		: 
		: "r0"
	);

	uprint("Read TrustZone NS Bit");
	if(scr_val & 0x1)
		uprint("TrustZone NS Bit is 1");
	else
		uprint("TrustZone NS Bit is 0");
	*/
	while(1);
    
	return(0);
}
