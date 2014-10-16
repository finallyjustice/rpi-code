#include "types.h"
#include "mmu.h"
#include "memlayout.h"
#include "versatile_pb.h"

volatile unsigned char * const UART0_BASE_TMP = (unsigned char *)0x101f1000;

extern void kmain(void);
extern void jump_stack(void);

void print_string(const char *str)
{
	while(*str != '\0')
	{
		*UART0_BASE_TMP = *str;
		str++;

		if(*str == '\n')
		{
			*UART0_BASE_TMP = '\r';
		}
	}
}

void copy_vectors(void) 
{
	extern unsigned int vector_start;
	extern unsigned int vector_end;
	unsigned int *vectors_src = &vector_start;
	unsigned int *vectors_dst = (unsigned int *)0;
 
	while(vectors_src < &vector_end)
	{
		*vectors_dst++ = *vectors_src++;
	}
}

void swi_handler(unsigned int arg)
{
	print_string("SWI Handler\n");
}

// kernel page table, reserved in the kernel.ld
extern uint32 _kernel_pgtbl;
extern uint32 _user_pgtbl;

uint32 *kernel_pgtbl = &_kernel_pgtbl;
uint32 *user_pgtbl = &_user_pgtbl;

#define PDE_SHIFT 20

// setup the boot page table: dev_mem whether it is device memory
void set_bootpgtbl(uint32 virt, uint32 phy, uint32 len, int dev_mem)
{
	uint32 pde;
	int    idx;

	// convert addr and len to index
	virt >>= PDE_SHIFT;
	phy  >>= PDE_SHIFT;
	len  >>= PDE_SHIFT;

	for(idx=0; idx<len; idx++)
	{
		pde = (phy << PDE_SHIFT);

		if (!dev_mem) 
		{
			// normal memory, make it kernel-only, cachable, bufferable
			pde |= (AP_KO << 10) | PE_CACHE | PE_BUF | KPDE_TYPE;
		} 
		else 
		{
			// device memory, make it non-cachable and non-bufferable
			pde |= (AP_KO << 10) | KPDE_TYPE;
		}

		// use different page table for user/kernel space
		if(virt < NUM_UPDE) 
		{
			user_pgtbl[virt] = pde;
		} 
		else 
		{
			kernel_pgtbl[virt] = pde;
		}

		virt++;
		phy++;
	}
}

void _flush_all(void)
{
	uint val = 0;

	// flush all TLB
	asm("MCR p15, 0, %[r], c8, c7, 0" : :[r]"r" (val):);
}

void load_pgtbl(uint32 *kern_pgtbl, uint32 *user_pgtbl)
{
	uint val;

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

void bootmain()
{
	//print_string("Hello World\n");
	//asm volatile("swi #0x1");
	print_string("Back from SWI!\n");

	set_bootpgtbl(0, 0, INIT_KERNMAP, 0);
	set_bootpgtbl(KERNBASE, 0, INIT_KERNMAP, 0);

	uint32  vectbl;
	// vector table is in the middle of first 1MB (0xF000)
	vectbl = P2V_WO (VEC_TBL & PDE_MASK);

	set_bootpgtbl(VEC_TBL, 0, 1 << PDE_SHIFT, 0);
	set_bootpgtbl(KERNBASE+DEVBASE, DEVBASE, DEV_MEM_SZ, 1);

	load_pgtbl(kernel_pgtbl, user_pgtbl);
	jump_stack();

	kmain();
}
