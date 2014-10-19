#include "io.h"
#include "uart.h"

void early_putc(unsigned int c)
{
	while(1)
	{
		if(EARLY_GET32(AUX_MU_LSR_REG) & 0x20)
		break;
	}
	EARLY_PUT32(AUX_MU_IO_REG, c);

	if(c == '\n')
		early_putc('\r');
}

void early_uart_init(void)
{
    unsigned int ra;

    EARLY_PUT32(AUX_ENABLES,     1);
    EARLY_PUT32(AUX_MU_IER_REG,  0);
    EARLY_PUT32(AUX_MU_CNTL_REG, 0);
    EARLY_PUT32(AUX_MU_LCR_REG,  3);
    EARLY_PUT32(AUX_MU_MCR_REG,  0);
    EARLY_PUT32(AUX_MU_IER_REG,  0);
    EARLY_PUT32(AUX_MU_IIR_REG,  0xC6);
    EARLY_PUT32(AUX_MU_BAUD_REG, 270);

    ra = EARLY_GET32(GPFSEL1);
    ra &= ~(7<<12);      // gpio14
    ra |= 2<<12;         // alt5
    ra &= ~(7<<15);      // gpio15
    ra |= 2<<15;         // alt5

    EARLY_PUT32(GPFSEL1, ra);
    EARLY_PUT32(GPPUD, 0);

    for(ra=0; ra<150; ra++) 
		EARLY_DUMMY(ra);
    
	EARLY_PUT32(GPPUDCLK0, (1<<14)|(1<<15));

    for(ra=0; ra<150; ra++) 
		EARLY_DUMMY(ra);

    EARLY_PUT32(GPPUDCLK0, 0);
    EARLY_PUT32(AUX_MU_CNTL_REG, 3);
}
