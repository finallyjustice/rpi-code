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

static void printint(int xx, int base, int sign)
{
	static char digits[] = "0123456789abcdef";
	char buf[16];
	int i;
	unsigned int x;

	if(sign && (sign = xx < 0))
		x = -xx;
	else
		x = xx;

	i = 0;
	do{
		buf[i++] = digits[x % base];
	}while((x /= base) != 0);

	if(sign)
		buf[i++] = '-';

	while(--i >= 0)
		uart_send(buf[i]);
}

// Print to the console. only understands %d, %x, %p, %s.
void cprintf(char *fmt, ...)
{
	int i, c;
	unsigned int *argp;
	char *s;

	argp = (unsigned int*)(void*)(&fmt + 1);
	for(i = 0; (c = fmt[i] & 0xff) != 0; i++)
	{
		if(c != '%')
		{
			if(c == '\n')
				uart_send('\r');
			uart_send(c);
			continue;
		}
		c = fmt[++i] & 0xff;
		if(c == 0)
			break;
		switch(c)
		{
		case 'd':
			printint(*argp++, 10, 1);
			break;
		case 'x':
		case 'p':
			printint(*argp++, 16, 0);
			break;
		case 's':
			if((s = (char*)*argp++) == 0)
				s = "(null)";
			for(; *s; s++)
				uart_send(*s);
			break;
		case '%':
			uart_send('%');
			break;
		default:
			// Print unknown % sequence to draw attention.
			uart_send('%');
			uart_send(c);
			break;
		}
	}
}


#define SYSTIMERCLO 0x20003004
#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028

// 1 m ticks per second

//0x01000000 17 seconds
//0x00400000 4 seconds
#define TIMER_BIT 0x00400000

// first "C" function
int bootmain(void)
{
	// init uart to enable debugging with TTL
    uart_init();

	unsigned int ra;

	//ra=GET32(GPFSEL1);
	//ra&=~(7<<18);
	//ra|=1<<18;
	//PUT32(GPFSEL1,ra);

	int i;
	int sum = 0;

	ra=GET32(SYSTIMERCLO);
	cprintf("time1: %d, %x\n", ra, ra);

	for(i=1; i<=100; i++)
		sum += i;

	ra=GET32(SYSTIMERCLO);
	cprintf("time2: %d, %x\n", ra, ra);

	cprintf("SUM: %d\n", sum);

	while(1)
	{
		PUT32(GPSET0,1<<16);
		while(1)
		{
			ra=GET32(SYSTIMERCLO);
			if((ra&=TIMER_BIT)==TIMER_BIT) break;
		}
		cprintf("First\n");
		PUT32(GPCLR0,1<<16);
		while(1)
		{
			ra=GET32(SYSTIMERCLO);
			if((ra&=TIMER_BIT)==0) break;
		}
		cprintf("Second\n");
	}
    
	return(0);
}
