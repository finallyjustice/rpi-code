
.globl _start
_start:
    b start_code

start_code:
    ldr sp, =svc_stktop
    bl bootmain
	b .

.globl EARLY_PUT32
EARLY_PUT32:
    str r1, [r0]
    bx lr

.globl EARLY_PUT16
EARLY_PUT16:
    strh r1, [r0]
    bx lr

.globl EARLY_PUT8
EARLY_PUT8:
    strb r1, [r0]
    bx lr

.globl EARLY_GET32
EARLY_GET32:
    ldr r0, [r0]
    bx lr

.globl EARLY_GETPC
EARLY_GETPC:
    mov r0, lr
    bx lr

.globl EARLY_BRANCHTO
EARLY_BRANCHTO:
    bx r0

.globl EARLY_DUMMY
EARLY_DUMMY:
    bx lr

# during startup, kernel stack uses user address, now switch it to kernel addr
.global jump_stack
jump_stack:
	MOV     r0, sp 
	ADD     r0, r0, #0x80000000
	MOV     sp, r0
	MOV     pc, lr
