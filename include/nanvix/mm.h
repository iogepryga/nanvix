/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This file is part of Nanvix.
 * 
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MM_H_
#define MM_H_
	
	#include <nanvix/const.h>
	#include <nanvix/hal.h>
	#include <nanvix/pm.h>
	#include <sys/types.h>
	
	/* Kernel stack size. */
	#define KSTACK_SIZE 4096

	/* Virtual memory layout. */
	#define UBASE_VIRT   0x00800000 /* User base.        */
	#define BUFFERS_VIRT 0xc0008000 /* Buffers.          */
	#define KBASE_VIRT   0xc0000000 /* Kernel base.      */
	#define KPOOL_VIRT   0xc0400000 /* Kernel page pool. */
	#define INITRD_VIRT  0xc0800000 /* Initial RAM disk. */
	
	/* Physical memory layout. */
	#define KBASE_PHYS   0x00000000 /* Kernel base.      */
	#define BUFFERS_PHYS 0x00008000 /* Buffers.          */
	#define KPOOL_PHYS   0x00400000 /* Kernel page pool. */
	#define UBASE_PHYS   0x00800000 /* User base.        */
	
	/* User memory layout. */
	#define USTACK_ADDR 0xc0000000 /* User stack. */
	#define UHEAP_ADDR  0xa0000000 /* User heap.  */

	/* Kernel memory size: 4 MB. */
	#define KMEM_SIZE 0x00400000
	
	/* Kernel page pool size: 4 MB. */
	#define KPOOL_SIZE 0x00400000
	
	/* User memory size. */
	#define UMEM_SIZE (MEMORY_SIZE - KMEM_SIZE - KPOOL_SIZE)

	/* Number of page frames. */
	#define NR_FRAMES (UMEM_SIZE/PAGE_SIZE)

	/**
 * @brief Gets a page directory entry of a process.
 * 
 * @param p Process.
 * @param a Address.
 * 
 * @returns The requested page directory entry.
 */
#define getpde(p, a) \
	(&(p)->pgdir[PGTAB(a)])

/**
 * @brief Gets a page table entry of a process.
 * 
 * @param p Process.
 * @param a Address.
 * 
 * @returns The requested page table entry.
 */
#define getpte(p, a) \
	(&((struct pte *)((getpde(p, a)->frame << PAGE_SHIFT) + KBASE_VIRT))[PG(a)])

#ifndef _ASM_FILE_

	struct mmframe
	{
		unsigned count; /**< Reference count.     */
		unsigned age;   /**< Age.                 */
		pid_t owner;    /**< Page owner.          */
		addr_t addr;    /**< Address of the page. */
	};
	
	EXTERN struct mmframe frames[NR_FRAMES];
	
	/* Forward definitions. */
	EXTERN int chkmem(const void *, size_t, mode_t);
	EXTERN int fubyte(const void *);
	EXTERN int fudword(const void *);
	EXTERN int crtpgdir(struct process *);
	EXTERN int pfault(addr_t);
	EXTERN int vfault(addr_t);
	EXTERN void dstrypgdir(struct process *);
	EXTERN void putkpg(void *);
	EXTERN void mm_init(void);
	EXTERN void *getkpg(int);

#endif /* _ASM_FILE_ */
	
#endif /* MM_H_ */
