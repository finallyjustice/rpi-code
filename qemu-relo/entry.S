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
	bl bootmain
	b .
