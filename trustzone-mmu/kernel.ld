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

		/* define the kernel page table, must be 16K and 16K-aligned*/
		. = ALIGN(0x4000);
		PROVIDE (_kernel_pgtbl = .);
		. += 0x4000;

		/* we also need a user page table*/
		PROVIDE (_user_pgtbl = .);
		. += 0x1000;
	}

	/*the kernel executes at the higher 2GB address space, but loaded
	  at the lower memory (0x20000)*/
	. = 0x80020000;

	.text : AT(0x20000){
		*(.text .text.* .gnu.linkonce.t.*)
	}

	PROVIDE(etext = .);   /* Define the 'etext' symbol to this value */

	.rodata : {
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}

	/* aligned the data to a (4K) page, so it can be assigned
	   different protection than the code*/
	. = ALIGN(0x1000);

	PROVIDE (data_start = .);

	.data : {
		*(.data .data.*)
	}

	PROVIDE (edata = .);

	.bss : {
		*(.bss .bss.* COMMON)
	}

	. = ALIGN(0x1000);
	PROVIDE (end = .);
}

