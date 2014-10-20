ENTRY(_start)

ENTRY_SVC_STACK_SIZE = 0x1000;

SECTIONS
{
	. = 0x8000;
	.start_sec : {
		secure_entry.o(.text)
		secure_start.o(.text .text.*)
		secure_uart.o(.text .text.*)
		normal_entry.o(.text .text.*)
		normal_start.o(.text .text.*)

		secure_entry.o(.rodata .rodata.*)
		secure_start.o(.rodata .rodata.*)
		secure_uart.o(.rodata .rodata.*)
		normal_entry.o(.rodata .rodata.*)
		normal_start.o(.rodata .rodata.*)

		secure_entry.o(.data .data.*)
		secure_start.o(.data .data.*)
		secure_uart.o(.data .data.*)
		normal_entry.o(.data .data.*)
		normal_start.o(.data .data.*)

		secure_entry.o(.bss .bss.* COMMON)
		secure_start.o(.bss .bss.* COMMON)
		secure_uart.o(.bss .bss.* COMMON)
		normal_entry.o(.bss .bss.* COMMON)
		normal_start.o(.bss .bss.* COMMON)

		/*define a stack for the entry*/
		. = ALIGN(0x1000);
		. += ENTRY_SVC_STACK_SIZE;
	
		PROVIDE (svc_stktop = .);
	}
}

