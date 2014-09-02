	.equ Mode_USR, 0x10
	.equ Mode_FIQ, 0x11
	.equ Mode_IRQ, 0x12
	.equ Mode_SVC, 0x13
	.equ Mode_ABT, 0x17
	.equ Mode_UND, 0x1B
	.equ Mode_SYS, 0x1F
	.equ Mode_MON, 0x16
	.equ I_Bit,    0x80 
	.equ F_Bit,    0x40
	
	.equ Len_FIQ_Stack,  64
	.equ Len_IRQ_Stack,  64
	.equ Len_ABT_Stack,  64
	.equ Len_UND_Stack,  64
	.equ Len_SVC_Stack,  512
	.equ Len_USR_Stack,  512

	.equ Offset_FIQ_Stack, 0
	.equ Offset_IRQ_Stack, Offset_FIQ_Stack + Len_FIQ_Stack
	.equ Offset_ABT_Stack, Offset_IRQ_Stack + Len_IRQ_Stack
	.equ Offset_UND_Stack, Offset_ABT_Stack + Len_ABT_Stack
	.equ Offset_SVC_Stack, Offset_UND_Stack + Len_UND_Stack
	.equ Offset_USR_Stack, Offset_SVC_Stack + Len_SVC_Stack

	.text
	.global _start
_start:
	b reset
	ldr pc, UndefAddr    
	ldr pc, SWIAddr      
	ldr pc, PAbortAddr
	ldr pc, DAbortAddr  
	ldr pc, ReservedAddr 
	ldr pc, IRQAddr 
	ldr pc, FIQAddr 

UndefAddr:      .word UndefHandler
SWIAddr:        .word SWIHandler
PAbortAddr:     .word PAbortHandler
DAbortAddr:     .word DAbortHandler
ReservedAddr:   .word 0
IRQAddr:        .word IRQHandler
FIQAddr:        .word FIQHandler

reset:
	ldr sp, =STACK_ADR
	bl bootmain

.global PUT32
PUT32:
	str r1, [r0]
	bx lr

.global PUT16
PUT16:
	strh r1, [r0]
	bx lr

.global PUT8
PUT8:
	strb r1, [r0]
	bx lr

.global GET32
GET32:
	ldr r0, [r0]
	bx lr

.global GETPC
GETPC:
	mov r0, lr
	bx lr

.global BRANCHTO
BRANCHTO:
	bx r0

.global dummy
dummy:
	bx lr

UndefHandler:
	b UndefHandler 
	
SWIHandler:
	b SWIHandler
 
PAbortHandler:
	b PAbortHandler

DAbortHandler:
	b DAbortHandler

IRQHandler:
	b IRQHandler

FIQHandler:
	b FIQHandler
