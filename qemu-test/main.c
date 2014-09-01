volatile unsigned char * const UART0_BASE = (unsigned char *)0x0101f1000;

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
	print("Bare Metal on QEMU");
}
