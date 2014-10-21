#include "uart.h"
#include "memlayout.h"
#include "types.h"
#include "mmu.h"
#include "pi.h"

extern void jump_stack();
extern void normal_main();

void uprint(char *s)
{
	int i = 0;
	while(s[i])
	{
		if(s[i] == '\n')
			sec_uart_send('\r');
		sec_uart_send(s[i]);
		i++;
	}
}

// kernel page table, reserved in the kernel.ld
extern uint32   _kernel_pgtbl;
extern uint32   _user_pgtbl;

uint32 *kernel_pgtbl = &_kernel_pgtbl;
uint32 *user_pgtbl = &_user_pgtbl;

// setup the boot page table: dev_mem whether it is device memory
void set_bootpgtbl (uint32 virt, uint32 phy, uint len, int dev_mem )
{
	uint32  pde;
	int     idx;

	// convert all the parameters to indexes
	virt >>= PDE_SHIFT;
	phy  >>= PDE_SHIFT;
	len  >>= PDE_SHIFT;

	 for (idx = 0; idx < len; idx++) 
	 {
		 pde = (phy << PDE_SHIFT);

		 if (!dev_mem) {
			 // normal memory, make it kernel-only, cachable, bufferable
			pde |= (AP_KO << 10) | PE_CACHE | PE_BUF | KPDE_TYPE;
		 } else {
			 // device memory, make it non-cachable and non-bufferable
			 pde |= (AP_KO << 10) | KPDE_TYPE;
		 }

		 // use different page table for user/kernel space
		 if (virt < NUM_UPDE) {
			 user_pgtbl[virt] = pde;
		 } else {
			 kernel_pgtbl[virt] = pde;
		 }

		 virt++;
		 phy++;
	 }
}

void _flush_all (void)
{
    uint val = 0;

    // flush all TLB
    asm("MCR p15, 0, %[r], c8, c7, 0" : :[r]"r" (val):);

    // invalid entire data and instruction cache
    asm ("MCR p15,0,%[r],c7,c10,0": :[r]"r" (val):);
    asm ("MCR p15,0,%[r],c7,c11,0": :[r]"r" (val):);
}

void load_pgtlb (uint32* kern_pgtbl, uint32* user_pgtbl)
{
    uint	val;

    // set domain access control: all domain will be checked for permission
    val = 0x55555555;
    asm("MCR p15, 0, %[v], c3, c0, 0": :[v]"r" (val):);

    // set the page table base registers. We use two page tables: TTBR0
    // for user space and TTBR1 for kernel space
    val = 32 - UADDR_BITS;
    asm("MCR p15, 0, %[v], c2, c0, 2": :[v]"r" (val):);

    // set the kernel page table
    val = (uint)kernel_pgtbl | 0x00;
    asm("MCR p15, 0, %[v], c2, c0, 1": :[v]"r" (val):);

    // set the user page table
    val = (uint)user_pgtbl | 0x00;
    asm("MCR p15, 0, %[v], c2, c0, 0": :[v]"r" (val):);

    // ok, enable paging using read/modify/write
    asm("MRC p15, 0, %[r], c1, c0, 0": [r]"=r" (val)::);

    val |= 0x80300D; // enable MMU, cache, write buffer, high vector tbl,
                     // disable subpage
    asm("MCR p15, 0, %[r], c1, c0, 0": :[r]"r" (val):);

    _flush_all();
}

void Normal_World()
{
	/*int i;
	for(i=0; i<10; i++)
	{
		uprint("Hello from new Normal world\n");
		asm volatile("smc #0\n\t");
	}

	uprint("Pending in normal world\n");
	while(1);*/

	uprint("Prepare first page table\n");

	// double map physical memory, required to enable paging
	set_bootpgtbl(0, 0, INIT_KERNMAP, 0);
	set_bootpgtbl(KERNBASE, 0, INIT_KERNMAP, 0);
	
	// map the device memory
	set_bootpgtbl(KERNBASE+DEVBASE, DEVBASE, DEV_MEM_SZ, 1);
	
	load_pgtlb (kernel_pgtbl, user_pgtbl);
	jump_stack ();  // move the stack to high memory

	normal_main();	
}
