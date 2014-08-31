
.globl _start
_start:
    b start_code

.space 0x200000-0x8004, 0

start_code:
    mov sp, #0x08000000
    bl bootmain
halt: b halt

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
