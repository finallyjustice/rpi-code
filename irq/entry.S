.global vectors_start
.global vectors_end

vectors_start:
	ldr pc, ResetAddr
	ldr pc, UndefAddr
	ldr pc, SWIAddr
	ldr pc, PAbortAddr
	ldr pc, DAbortAddr
	ldr pc, IRQAddr
	ldr pc, FIQAddr

ResetAddr:  .word ResetHandler
UndefAddr:  .word UndefHandler
SWIAddr:    .word SWIHandler
PAbortAddr: .word PAbortHandler
DAbortAddr: .word DAbortHandler
IRQAddr:    .word IRQHandler
FIQAddr:    .word FIQHandler
vectors_end:

ResetHandler:
	ldr sp, =STACK_ADR
	bl copy_vectors
	bl bootmain
	b .

UndefHandler:
	b .
	
SWIHandler:
	push {lr}
	ldr r0,[lr,#-4]
	bl swi_handler
	pop {lr}
	movs pc, lr

PAbortHandler:
	b .

DAbortHandler:
	b .

IRQHandler:
	b .

FIQHandler:
	b .


.globl PUT32
PUT32:
    str r1, [r0]
    bx lr

.globl PUT16
PUT16:
    strh r1, [r0]
    bx lr

.globl PUT8
PUT8:
    strb r1, [r0]
    bx lr

.globl GET32
GET32:
    ldr r0, [r0]
    bx lr

.globl GETPC
GETPC:
    mov r0, lr
    bx lr

.globl BRANCHTO
BRANCHTO:
    bx r0

.globl dummy
dummy:
    bx lr
