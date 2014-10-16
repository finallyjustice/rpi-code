#include "memlayout.h"

.global vector_start
.global vector_end

.global _start
_start:
vector_start:
	ldr pc, ResetAddr
	ldr pc, UndefAddr
	ldr pc, SWIAddr
	ldr pc, PAbortAddr
	ldr pc, DAbortAddr
	ldr pc, IRQAddr
	ldr pc, FIQAddr

ResetAddr:	.word ResetHandler
UndefAddr:	.word UndefHandler
SWIAddr:	.word SWIHandler
PAbortAddr:	.word PAbortHandler
DAbortAddr:	.word DAbortHandler
IRQAddr:	.word IRQHandler
FIQAddr:	.word FIQHandler
vector_end:

ResetHandler:
	ldr sp, =svc_stktop
	bl copy_vectors
	bl bootmain
#b .

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

# during startup, kernel stack uses user address, now switch it to kernel addr
.global jump_stack
jump_stack:
	MOV     r0, sp
	ADD     r0, r0, #0x80000000
	MOV     sp, r0
	MOV     pc, lr
