ENTRY(_start)

ENTRY_SVC_STACK_SIZE = 0x1000;

SECTIONS
{
	. = 0x8000;
	.start_sec : {
		entry.o(.text)
		main.o(.text .text.*)
		uart.o(.text .text.*)
		normal_entry.o(.text .text.*)
		normal_main.o(.text .text.*)

		entry.o(.rodata .rodata.*)
		main.o(.rodata .rodata.*)
		uart.o(.rodata .rodata.*)
		normal_entry.o(.rodata .rodata.*)
		normal_main.o(.rodata .rodata.*)

		entry.o(.data .data.*)
		main.o(.data .data.*)
		uart.o(.data .data.*)
		normal_entry.o(.data .data.*)
		normal_main.o(.data .data.*)

		entry.o(.bss .bss.* COMMON)
		main.o(.bss .bss.* COMMON)
		uart.o(.bss .bss.* COMMON)
		normal_entry.o(.bss .bss.* COMMON)
		normal_main.o(.bss .bss.* COMMON)

		/*define a stack for the entry*/
		. = ALIGN(0x1000);
		. += ENTRY_SVC_STACK_SIZE;
	
		PROVIDE (svc_stktop = .);
	}
}

