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

void Normal_World()
{
	while(1) 
	{
		int rb = GET32(SYSTIMERCLO);
		cprintf("hello from Normal world: %d %x\n", rb, rb);
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

	for(i=0; i<10; i++)	
	{
		int ra=GET32(SYSTIMERCLO);
		cprintf("hello from Secure world\n");
		asm volatile("smc #0\n\t");
		cprintf("time in secure: %d %x\n", ra, ra);
	};

	while(1);
}
