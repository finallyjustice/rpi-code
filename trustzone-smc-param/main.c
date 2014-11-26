#include "io.h"
#include "uart.h"
#include <stdint.h>

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
		case 'c':
			uart_send((char) *argp++);
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

void secure_swi() 
{
	cprintf("In Secure World's SWI Handler\n");
}
void ns_reset() 
{
	cprintf("In Normal World's reset Handler\n");
}
void ns_undef() 
{
	cprintf("In Normal World's Undef Handler\n");
}
void ns_swi() 
{
	cprintf("In Normal World's SWI Handler\n");
}
void ns_pa() 
{
	cprintf("In Normal World's PA Handler\n");
}
void ns_da() 
{
	cprintf("In Normal World's SWI Handler\n");
}
void ns_irq() 
{
	cprintf("In Normal World's IRQ Handler\n");
}
void ns_fiq() 
{
	cprintf("In Normal World's FIQ Handler\n");
}

void smc_print(unsigned int c, unsigned int c2, unsigned int c3, 
unsigned int c4, unsigned int c5, unsigned int c6 ) {

	cprintf("SMC R0 val :%x\n", c);
	cprintf("SMC R1 val :%x\n", c2);
	cprintf("SMC R2 val :%x\n", c3);
	cprintf("SMC R3 val :%x\n", c4);
	cprintf("SMC R4 val :%x\n", c4);
	cprintf("SMC R5 val :%x\n", c4);
}
/*void smc_print() {
	void * resv_sp = (void *)0x90000;
	cprintf("SMC R0 val :%x\n", *(int *)resv_sp--);
	cprintf("SMC R1 val :%x\n", *(int *)resv_sp--);
	cprintf("SMC R3 val :%x\n", *(int *)resv_sp--);
	resv_sp = (void *)0x90000;
	cprintf("SMC R0 val :%x\n", *(int *)resv_sp++);
	cprintf("SMC R1 val :%x\n", *(int *)resv_sp++);
	cprintf("SMC R3 val :%x\n", *(int *)resv_sp++);
	//cprintf("SMC R3 val :%x\n", *(int *)resv_sp);
}*/
void Normal_World()
{
	while(1) 
	{
		int rb = GET32(SYSTIMERCLO);
		cprintf("hello from Normal world: %d %x\n", rb, rb);
		cprintf("Before Normal SWI\n");
		asm volatile("swi #0\n\t");
		cprintf("After Normal SWI\n");
		asm volatile("ldr r0, =200\n\t"
			"ldr r1, =201\n\t"
			"ldr r2, =202\n\t"
			"ldr r3, =203\n\t"
			"ldr r4, =204\n\t"
			"ldr r5, =205\n\t"
			"smc #5\n\t");
		
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
	uprint("\n\n**********Exp Start***********\n");
	hexstring(arr[10]);
	uprint("Test TrustZone v2\n");
	uprint("Hello World\n");

	monitorInit(Normal_World);
	
	uprint("Install Monitor Successfully\n");

	for(i=0; i<10; i++)	
	{
		cprintf("hello from Secure world\n");
		int ra=GET32(SYSTIMERCLO);
		asm volatile( "ldr r0, =100\n\t"
			"ldr r1, =101\n\t"
			"ldr r2, =102\n\t"
			"ldr r3, =103\n\t"
			"ldr r4, =104\n\t"
			"ldr r5, =105\n\t"
			"smc #6\n\t");
		cprintf("time in secure: %d %x\n", ra, ra);
		cprintf("Before Secure SWI\n");
		asm volatile("swi #0\n\t");
		cprintf("After Secure SWI\n");
	};

	while(1);
}
