#include "io.h"
#include "uart.h"

#define MY_AUX_MU_LSR_REG  0xA0215054
#define MY_AUX_MU_IO_REG   0xA0215040

void test_send(unsigned int c)
{
	while(1)
	{
		if(GET32(MY_AUX_MU_LSR_REG) & 0x20)
			break;
	}
		
	PUT32(MY_AUX_MU_IO_REG, c);
}

void kmain(void)
{
	cprintf("Hello after paging!!!\n");
	test_send('K');

	while(1);
	return;
}
