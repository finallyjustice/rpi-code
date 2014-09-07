/* last 5 bit of Program Status Register (PSR) */
	.equ Mode_USR, 0x10 @ User Mode
	.equ Mode_FIQ, 0x11 @ Fast Interrupt Mode
	.equ Mode_IRQ, 0x12 @ Interrupt Mode
	.equ Mode_SVC, 0x13 @ Supervisor Mode
	.equ Mode_ABT, 0x17 @ Abort Mode
	.equ Mode_UND, 0x1B @ Undefined Mode
	.equ Mode_SYS, 0x1F @ System Mode
	.equ Mode_MON, 0x16 @ Monitor Mode

/* 6th and 7th bit of Program Status Register (PSR) */
	.equ I_Bit,    0x80 @ IRQ interrupts disabled 
	.equ F_Bit,    0x40 @ FIQ interrupts disabled

	.equ NS_BIT,   0x1

/* memory reserved (in bytes) for stacks of different mode */
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

UndefAddr:		.word UndefHandler
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
	
/* Init stack pointer registers for each mode
 * Enter each mode in turn and set up the stack pointer 
 */

	ldr r0, =STACK_ADDR

	cps #Mode_MON
	ldr sp, =STACK_ADDR

	msr cpsr_c, #Mode_FIQ | I_Bit | F_Bit /* Disable interrupts*/
	sub sp, r0, #Offset_FIQ_Stack
 
	msr cpsr_c, #Mode_IRQ | I_Bit | F_Bit /* Disable interrupts */
	sub     sp, r0, #Offset_IRQ_Stack
	 
	msr cpsr_c, #Mode_ABT | I_Bit | F_Bit /* Disable interrupts */
	sub sp, r0, #Offset_ABT_Stack
	
	msr cpsr_c, #Mode_UND | I_Bit | F_Bit  
	sub sp, r0, #Offset_UND_Stack   
	
	msr cpsr_c, #Mode_SVC | I_Bit | F_Bit /* Disable interrupts */
	sub sp, r0, #Offset_SVC_Stack

	bl bootmain

CPU_SVC32_MODE:
	mrs r0, cpsr        /* read cpsr register */
	bic r0, r0, #0x1f   /* clear last 5 bits mode */
	orr r0, r0, #0xd3   /* disable FIQ&IRQ and set cpu svc mode*/
	msr cpsr, r0
	bx lr

Disable_MMU_L1cache:
	mrc p15, 0, r0, c1, c0, 0    /* read CP15 register 1 into r0 */
	bic r0, r0, #(0x1<<13)		 /* clear bit 13 */
	bic r0, r0, #(0x1<<12)		 /* disable I Cache */
	bic r0, r0, #(0x1<<2)		 /* disable D Cache */
	bic r0, r0, #(0x1<<0)		 /* disable MMU */
	mcr p15, 0, r0, c1, c0, 0    /* write CP15 register 1 */
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

.global monitor
.align 5
monitor:
	@ Monitor
	nop @ Reset		 - not used by Monitor
	nop @ Undef      - not used by Monitor
	B   smc_handler
	nop @ Prefetch   - can by used by Monitor
	nop @ Data abort - can by used by Monitor
	nop @ RESERVED
	nop @ IRQ        - can by used by Monitor
	nop @ FIQ        - can by used by Monitor

/* 
 * SMC Handler
 *
 * - Detect which world executed SMC
 * - Saves state to appropriate stack
 * - Restores other worlds state
 * - Switches world
 * - Performs exception return
 */

.global smc_handler
smc_handler:
	push {r0-r3}							@ R0-r3 contain args to be passed between worlds
											@ Temporarily stack, so can be used as scratch regs

	@ Which world have we come from
	@ ------------------------------
	mrc p15, 0, r0, c1, c1, 0				@ Read Secure Configuration Register data
	tst r0, #NS_BIT							@ Is the NS bit set?
	eor r0, r0, #NS_BIT						@ Toggle NS bit
	mcr p15, 0, r0, c1, c1, 0				@ Write Secure Configuration Register data

	@ Load save to pointer
	@ ---------------------
	ldreq  r0, =S_STACK_SP					@ If NS bit set, was in Normal world.  So restore Secure state
	ldrne  r0, =NS_STACK_SP
	ldr    r2, [r0]

	@ Load restore from pointer
	@ --------------------------
	ldreq  r1, =NS_STACK_SP
	ldrne  r1, =S_STACK_SP
	ldr    r3, [r1]

	@ r2  <-- save to
	@ r3  <-- restore from
 
	@ Save general purpose registers, SPSR and LR
	@ --------------------------------------------
	stmfd   r2!, {r4-r12}					@ Save r4 to r12
	@ ADD SUPPORT FOR SPs
	mrs     r4, spsr						@ Also get a copy of the SPSR
	stmfd   r2!, {r4, lr}					@ Save original SPSR and LR
	
	str     r2, [r0]						@ Save updated pointer back, r0 and r2 now free
	
	@ Restore other world registers, SPSR and LR
	@ ---------------------------------------------
	ldmfd   r3!, {r0, lr}					@ Get SPSR and LR from
	@ ADD SUPPORT FOR SPs
	msr     spsr_cxsf, r0					@ Restore SPSR
	ldmfd   r3!, {r4-r12}					@ Restore registers r4 to r12

	str r3, [r1]							@ Save updated pointer back, r1 and r3 now free
	
	@ Clear local monitor
	@ --------------------
	clrex									@ Not strictly required in this example, as not using LDR EX/STREX
											@ However, architecturally should execute CLREX on a context switch

	@ Now restore args (r0-r3)
	@ -------------------------
	pop {r0-r3}
	
	
	@ Perform exception return
	@ -------------------------
	movs pc, lr

/*
 * Monitor Initialization
 *
 * This is called the first time the Secure world wishes to
 * transit to the Normal world.
 */

.global init_secure_monitor
init_secure_monitor:
	@ Install Secure Monitor
	@ -----------------------
	ldr r1, =ns_image					@ R1 is used
	str r0, [r1]
	ldr r0, =monitor					@ Get address of Monitors vector table
	mcr p15, 0, r0, c12, c0, 1			@ Write Monitor Vector Base Address Register

	@ Save Secure state
	@ ------------------
	ldr    r0, =S_STACK_LIMIT			@ Get address of Secure state stack
	stmfd  r0!, {r4-r12}				@ Save general purpose registers
	@ ADD support for SPs
	mrs    r1, cpsr						@ Also get a copy of the CPSR
	stmfd  r0!, {r1, lr}				@ Save CPSR and LR

	@ Switch to Monitor mode
	@ -----------------------
	cps    #Mode_MON					@ Move to Monitor mode after saving Secure state

	@ Save Secure state stack pointer
	@ --------------------------------
	ldr r1, =S_STACK_SP					@ Get address of global
	str r0, [r1]						@ Save pointer


	@ Set up initial NS state stack pointer
	@ --------------------------------------
	ldr r0, =NS_STACK_SP				@ Get address of global
	ldr r1, =NS_STACK_LIMIT				@ Get top of Normal state stack (assuming FD model)
	str r1, [r0]						@ Save pointer


	@ Set up exception return information
	@ ------------------------------------
	@IMPORT  ns_image
	 
	ldr lr, ns_image					@ ns_image
	msr spsr_cxsf, #Mode_SVC			@ Set SPSR to be SVC mode

	@ Switch to Normal world
	@ -----------------------
	mrc p15, 0, r4, c1, c1, 0			@ Read Secure Configuration Register data
	orr r4, #NS_BIT						@ Set NS bit
	mcr p15, 0, r4, c1, c1, 0			@ Write Secure Configuration Register data
	 
	 
	@ Clear general purpose registers
	@ --------------------------------
	mov r0,  #0
	mov r1,  #0
	mov r2,  #0
	mov r3,  #0
	mov r4,  #0
	mov r5,  #0
	mov r6,  #0
	mov r7,  #0
	mov r8,  #0
	mov r9,  #0
	mov r10, #0
	mov r11, #0
	mov r12, #0
 
	movs pc, lr


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

.global DUMMY
DUMMY:
	bx lr

@ ------------------------------------------------------------
@ Space reserved for secure and non-secure stacks
@ ------------------------------------------------------------
NS_STACK_BASE:
	.word     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
NS_STACK_LIMIT:

S_STACK_BASE:
	.word     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
S_STACK_LIMIT:

NS_STACK_SP:
	.word     0
	
S_STACK_SP:
	.word     0

ns_image :
	.word     0

@.end
@ ------------------------------------------------------------
@ End
@ ------------------------------------------------------------

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
