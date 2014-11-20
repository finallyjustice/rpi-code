volatile unsigned char * const UART0_BASE = (unsigned char *)0x0101f1000;

void uart_send(unsigned int c)
{
	*UART0_BASE = c;
}

void printint(int xx, int base, int sign)
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

void printf(char *fmt, ...)
{
	int i, c;
	unsigned int *argp;
	char *s;

	argp = (unsigned int*)(void*)(&fmt + 1);
	for(i = 0; (c = fmt[i] & 0xff) != 0; i++)
	{
		if(c != '%')
		{
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

extern unsigned long _norm_begin;
extern unsigned long _norm_end;

int flag = 0;

void bootmain()
{
	if(flag == 2)
		while(1);
	flag++;

	unsigned long *norm_begin = &_norm_begin;
	unsigned long *norm_end   = &_norm_end;

	unsigned char *new_func = 0x30000;
	unsigned char *old_func = (unsigned char *)norm_begin;

	unsigned char *ca = old_func;
	int i = 0;
	for(ca=old_func; ca<=norm_end; ca++)
	{
		new_func[i] = *ca;
		i++;
	}
	
	//int (*f) (void) = norm_begin;
	int (*f) (void) = 0x30000;

	printf("Hello World %d\n", 333);
	printf("norm_begin : 0x%x\n", norm_begin);
	printf("norm_end   : 0x%x\n", norm_end);
	f();
}
