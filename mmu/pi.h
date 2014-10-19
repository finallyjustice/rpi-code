// board configuration for raspberry pi (BCM2835_ARM_peripherals.pdf) 
#ifndef PI_HEAD
#define PI_HEAD


// just assume we have 128M memory for now, need a way to discover it

#define PHYSTOP         0x08000000


#define DEVBASE         0x20000000
#define DEV_MEM_SZ      0x01000000
#define VEC_TBL         0xFFFF0000


#define STACK_FILL      0xdeadbeef

#define SYSTIMER_BASE   0x20003000
#define CLK_HZ          1000000     // the clock is 1MHZ

// Interrupt controller and interrupt source
#define VIC_BASE        0x2000B200

#define PIC_TIMER0      0
#define PIC_MBOX        1
#define PIC_DOORBELL0   2
#define PIC_DOORBELL1   3
#define PIC_GPU0        4
#define PIC_GPU1        5
#define PIC_ILLEGAL0    7
#define PIC_ILLEGAL1    6

#define SYS_TIMER3      35

#endif