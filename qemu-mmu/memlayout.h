#ifndef __MEMLAYOUT_H__
#define __MEMLAYOUT_H__

#define KERNBASE        0x80000000
#define INIT_KERNMAP    0x100000

#define V2P(a) (((uint) (a)) - KERNBASE)
#define P2V(a) (((void *) (a)) + KERNBASE)

#define V2P_WO(x) ((x) - KERNBASE)    // same as V2P, but without casts
#define P2V_WO(x) ((x) + KERNBASE)    // same as V2P, but without casts

#endif
