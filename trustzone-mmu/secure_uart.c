#include "io.h"
#include "uart.h"

// GPIO14 TXD0 and TXD1
// GPIO15 RXD0 and RXD1
unsigned int sec_uart_lcr(void)
{
	return(SEC_GET32(AUX_MU_LSR_REG));
}

unsigned int sec_uart_recv(void)
{
    while(1)
    {
        if(SEC_GET32(AUX_MU_LSR_REG) & 0x01) 
			break;
    }

    return (SEC_GET32(AUX_MU_IO_REG) & 0xFF);
}

void sec_uart_send(unsigned int c)
{
    while(1)
    {
        if(SEC_GET32(AUX_MU_LSR_REG) & 0x20) 
			break;
    }

    SEC_PUT32(AUX_MU_IO_REG, c);
}

void sec_uart_flush(void)
{
    while(1)
    {
        if((SEC_GET32(AUX_MU_LSR_REG)&0x100) == 0) 
			break;
    }
}

void uart_init(void)
{
    unsigned int ra;

    SEC_PUT32(AUX_ENABLES,     1);
    SEC_PUT32(AUX_MU_IER_REG,  0);
    SEC_PUT32(AUX_MU_CNTL_REG, 0);
    SEC_PUT32(AUX_MU_LCR_REG,  3);
    SEC_PUT32(AUX_MU_MCR_REG,  0);
    SEC_PUT32(AUX_MU_IER_REG,  0);
    SEC_PUT32(AUX_MU_IIR_REG,  0xC6);
    SEC_PUT32(AUX_MU_BAUD_REG, 270);

    ra = SEC_GET32(GPFSEL1);
    ra &= ~(7<<12);      // gpio14
    ra |= 2<<12;         // alt5
    ra &= ~(7<<15);      // gpio15
    ra |= 2<<15;         // alt5

    SEC_PUT32(GPFSEL1, ra);
    SEC_PUT32(GPPUD, 0);

    for(ra=0; ra<150; ra++) 
		SEC_DUMMY(ra);
    
	SEC_PUT32(GPPUDCLK0, (1<<14)|(1<<15));

    for(ra=0; ra<150; ra++) 
		SEC_DUMMY(ra);

    SEC_PUT32(GPPUDCLK0, 0);
    SEC_PUT32(AUX_MU_CNTL_REG, 3);
}
