.global _start
_start:
	b ResetHandler
	b .
	b .
	b .
	b .
	b .
	b .
	b .

ResetHandler:
	ldr sp, =stack_top
	ldr	r0, =MESSAGE
	bl as_print
	bl bootmain
	b .

SWIHandler:
	ldr sp, =stack_top
	ldr r0, =MESSAGE
	bl as_print
	b .
	bl swi_handler
	b .

# assembly (AS) version of print
.global	as_print
as_print:
	# r0 is the param, the addr of char *
	ldrb r3, [r0, #0]
	cmp r3, #0
	bxeq lr
	ldr r2, =AS_UART0_BASE
	ldr r2, [r2, #0]
loop:
	str	r3, [r2, #0]
	ldrb r3, [r0, #1]!
	cmp	r3, #0
	bne	loop
	bx	lr

.section .rodata
.global	AS_UART0_BASE
AS_UART0_BASE:
	.word 0x101F1000
MESSAGE:
	.ascii "Print in Assembly!\012\000"
