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
	bl CPU_SVC32_MODE
	bl Disable_MMU_L1cache
	bl Init_aips
	bl Init_clock
	
/* --- Initialize stack pointer registers
	   Enter each mode in turn and set up the stack pointer */

	ldr     r0, =STACK_ADR

	CPS     #Mode_MON
	LDR     sp, =STACK_ADR

	MSR     CPSR_c, #Mode_FIQ | I_Bit | F_Bit /* No interrupts*/
	SUB     sp, r0, #Offset_FIQ_Stack
 
	MSR     CPSR_c, #Mode_IRQ | I_Bit | F_Bit /* No interrupts */
	SUB     sp, r0, #Offset_IRQ_Stack
	 
	MSR     CPSR_c, #Mode_ABT | I_Bit | F_Bit /* No interrupts */
	SUB     sp, r0, #Offset_ABT_Stack
	
	MSR     CPSR_c, #Mode_UND | I_Bit | F_Bit  
	SUB     sp, r0, #Offset_UND_Stack   
	
	MSR     CPSR_c, #Mode_SVC | I_Bit | F_Bit /* No interrupts */
	SUB     sp, r0, #Offset_SVC_Stack

/* Init .bss */
/* Clear the .bss section (zero init) */
/*
	LDR     r1,=__bss_start
	LDR     r2,=__bss_end
	MOV     r3,#0
1:
	CMP     r1,r2
	STMLTIA r1!,{r3}
	BLT     1b
*/

#ldr sp, =STACK_ADR
	bl bootmain

CPU_SVC32_MODE:
	mrs r0, cpsr        /*read cpsr register status*/
	bic r0, r0, #0x1f   /*cpsr mode bit clear*/
	orr r0, r0, #0xd3   /*FIQ&IRQ Disable and set cpu svc mode*/
	msr cpsr, r0
	bx lr

Disable_MMU_L1cache:
	mrc     p15, 0, r0, c1, c0, 0    /* read CP15 register 1 into r0 */
	bic     r0, r0, #(0x1  <<13)     /* clear bit 13 */
	bic     r0, r0, #(0x1  <<12)     /* disable I Cache */
	bic     r0, r0, #(0x1  <<2)      /* disable D Cache */
	bic     r0, r0, #(0x1  <<0)      /* disable MMU */
	mcr     p15, 0, r0, c1, c0, 0    /* write CP15 register 1 */
	bx lr

Init_aips:
	ldr r0, =0x0207C000
	ldr r1, =0x77777777
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
	ldr r1, =0x0
	str r1, [r0, #0x40]
	str r1, [r0, #0x44]
	str r1, [r0, #0x48]
	str r1, [r0, #0x4C]
	str r1, [r0, #0x50]

	ldr r0, =0x0217C000
	ldr r1, =0x77777777
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
	ldr r1, =0x0
	str r1, [r0, #0x40]
	str r1, [r0, #0x44]
	str r1, [r0, #0x48]
	str r1, [r0, #0x4C]
	str r1, [r0, #0x50]
	bx lr

Init_clock:
	/* Restore the default values in the Gate registers */
	ldr r0, =0x020c4000
	ldr r1, =0xC0003F
	str r1, [r0, #0x68]
	ldr r1, =0x30FC00
	str r1, [r0, #0x6c]
	ldr r1, =0xFFFC000
	str r1, [r0, #0x70]
	ldr r1, =0x3FF00000
	str r1, [r0, #0x74]
	ldr r1, =0xFFF300
	str r1, [r0, #0x78]
	ldr r1, =0xF0000C3
	str r1, [r0, #0x7c]
	ldr r1, =0x3FC
	str r1, [r0, #0x80]
	bx lr

	.ltorg
	.section .init, "ax"
	.code 32
	
	Mode_MON            =   0x16
	Mode_SVP            =   0x13
	NS_BIT              =   0x1

.global monitor
.align 5
monitor:
	@ Monitor
	NOP     @ Reset      - not used by Monitor
	NOP     @ Undef      - not used by Monitor
	B       SMC_Handler
	NOP     @ Prefetch   - can by used by Monitor
	NOP     @ Data abort - can by used by Monitor
	NOP     @ RESERVED
	NOP     @ IRQ        - can by used by Monitor
	NOP     @ FIQ        - can by used by Monitor

@ ------------------------------------------------------------
@ SMC Handler
@
@ - Detect which world executed SMC
@ - Saves state to appropriate stack
@ - Restores other worlds state
@ - Switches world
@ - Performs exception return
@ ------------------------------------------------------------

.global SMC_Handler
SMC_Handler:
	PUSH   {r0-r3}                       @ R0-r3 contain args to be passed between worlds
										 @ Temporarily stack, so can be used as scratch re    gs
										 
	@ Which world have we come from
	@ ------------------------------
	MRC     p15, 0, r0, c1, c1, 0        @ Read Secure Configuration Register data
	TST     r0, #NS_BIT                  @ Is the NS bit set?
	EOR     r0, r0, #NS_BIT              @ Toggle NS bit
	MCR     p15, 0, r0, c1, c1, 0        @ Write Secure Configuration Register data

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
