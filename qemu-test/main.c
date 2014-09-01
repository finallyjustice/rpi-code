volatile unsigned char * const UART0_BASE = (unsigned char *)0x0101f1000;

// the version of print implemented in assembly
extern void as_print(char *str);

void print(const char *str)
{
	while(*str != '\0')
	{
		*UART0_BASE = *str;
		str++;
	}
}

void bootmain()
{
	print("Hello World\n");
	as_print("Bare Metal on QEMU");
}
