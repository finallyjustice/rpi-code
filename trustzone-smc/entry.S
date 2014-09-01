
.globl _start
_start:
    b start_code

.space 0x200000-0x8004, 0

start_code:
    mov sp, #0x08000000
    bl bootmain
halt: b halt

.global monitor
.align 5
monitor:
	nop     @ Reset      - not used by Monitor
	nop     @ Undef      - not used by Monitor
	bl      smc_handler
	nop     @ Prefetch   - can by used by Monitor
    nop     @ Data abort - can by used by Monitor
	nop     @ RESERVED
	nop     @ IRQ        - can by used by Monitor
	nop     @ FIQ        - can by used by Monitor

.global setup_monitor
setup_monitor:
	ldr r0, =monitor
	mcr p15, 0, r0, c12, c0, 1
	bx lr

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
