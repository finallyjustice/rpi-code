#include "io.h"
#include "uart.h"

// GPIO14 TXD0 and TXD1
// GPIO15 RXD0 and RXD1
unsigned int uart_lcr(void)
{
	return(GET32(AUX_MU_LSR_REG));
}

unsigned int uart_recv(void)
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG) & 0x01) 
			break;
    }

    return (GET32(AUX_MU_IO_REG) & 0xFF);
}

void uart_send(unsigned int c)
{
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG) & 0x20) 
			break;
    }

    PUT32(AUX_MU_IO_REG, c);
}

void uart_flush(void)
{
    while(1)
    {
        if((GET32(AUX_MU_LSR_REG)&0x100) == 0) 
			break;
    }
}

void hexstring(unsigned int d)
{
	hexstrings(d);
	uart_send(0x0D);
	uart_send(0x0A);
}

void hexstrings(unsigned int d)
{
	unsigned int rb;
	unsigned int rc;
	
	rb = 32;
	while(1)
	{
		rb -= 4;
		rc = (d>>rb)&0xF;
		if(rc > 9) 
			rc += 0x37; 
		else 
			rc += 0x30;
		uart_send(rc);
		if(rb == 0) 
			break;
	}	
	uart_send(0x20);
}

void uart_init(void)
{
    unsigned int ra;

    PUT32(AUX_ENABLES,     1);
    PUT32(AUX_MU_IER_REG,  0);
    PUT32(AUX_MU_CNTL_REG, 0);
    PUT32(AUX_MU_LCR_REG,  3);
    PUT32(AUX_MU_MCR_REG,  0);
    PUT32(AUX_MU_IER_REG,  0);
    PUT32(AUX_MU_IIR_REG,  0xC6);
    PUT32(AUX_MU_BAUD_REG, 270);

    ra = GET32(GPFSEL1);
    ra &= ~(7<<12);      // gpio14
    ra |= 2<<12;         // alt5
    ra &= ~(7<<15);      // gpio15
    ra |= 2<<15;         // alt5

    PUT32(GPFSEL1, ra);
    PUT32(GPPUD, 0);

    for(ra=0; ra<150; ra++) 
		dummy(ra);
    
	PUT32(GPPUDCLK0, (1<<14)|(1<<15));

    for(ra=0; ra<150; ra++) 
		dummy(ra);

    PUT32(GPPUDCLK0, 0);
    PUT32(AUX_MU_CNTL_REG, 3);
}
