#include "uart.h"
#include "io.h"
#include "memlayout.h"

void test_send(unsigned int c)
{
	while(1)
	{
		if(GET32(_P2V(AUX_MU_LSR_REG)) & 0x20)
			break;
	}
		 
	PUT32(_P2V(AUX_MU_IO_REG), c);
}

void test_uprint(char *s)
{
	int i = 0;
	while(s[i])
	{
		if(s[i] == '\n')
			test_send('\r');
		test_send(s[i]);
		i++;
	}
}


void normal_main(void)
{
	test_uprint("normal main!\n");	

	int i;
	for(i=0; i<10; i++)
	{
		test_uprint("Hello from new Normal world\n");
		asm volatile("smc #0\n\t");
	}
	
	test_uprint("Pending in normal world\n");
	while(1);
}
