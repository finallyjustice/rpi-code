MEMORY
{
    ram : ORIGIN = 0x8000, LENGTH = 0x1000000
}

SECTIONS
{
    .text : { *(.text*) } > ram
    .bss : { *(.bss*) } > ram
	
	. = 0xc0000000;
	.stack :
	{
		STACK_ADR = .;
	}
}

