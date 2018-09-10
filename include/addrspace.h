#ifndef _ADDRSPACE_H_
#define _ADDRSPACE_H_

#include <vm.h>
/* synch.h has declaration of bool */
#include <synch.h>
#include "opt-dumbvm.h"

struct vnode;
extern struct lock *coremap_lock;
extern struct cm_entry* coremap;
/* 
 * Address space - data structure associated with the virtual memory
 * space of a process.
 *
 * You write this.
 */

/***********/
/*  Pages  */
/***********/
 struct page_struct {
	paddr_t p_addr:20;
	bool dirty:1;
	bool writable:1; 
	bool valid:1;
	//int dummy:9;
};

/*******************/
/* Coremap Entry   */
/*******************/
 struct cm_entry {
	struct page_struct page;  
	vaddr_t v_addr;
	unsigned int cpu_index:4;
	int tlb_index:7;
	int part_of_sequence:8; 
    bool is_kernel_page:1;
	bool is_allocated:1;
	bool is_pinned:1; //page is busy
    bool is_last_page:1;

};

/* Second level page table */
struct secondlevel_pagetable {
	struct page_struct pagetable_entry[1024];
};

/* Master level page table */
struct masterlevel_pagetable { 
	struct secondlevel_pagetable *leveltwo_pagetable[1024];
};

struct addrspace {
	/* Keep track of the heap */
	u_int32_t heap_start;
	u_int32_t heap_end;
	/* Keep track of the stack */
	size_t file_size1;
	size_t mem_size1;
	off_t as_offset1; 
	vaddr_t as_vbase1;
	vaddr_t as_vtop1;
	paddr_t as_pbase1;
	size_t as_npages1;
	size_t file_size2;
	size_t mem_size2;
	off_t as_offset2; 
	vaddr_t as_vbase2;
	vaddr_t as_vtop2;
	paddr_t as_pbase2;
	size_t as_npages2;
	u_int32_t stack_end; 
	vaddr_t as_stackpbase;
	struct vnode *as_v;
	/* First level page table for each process/thread */
	struct masterlevel_pagetable pagetable; 
};


/*
 * Functions in addrspace.c:
 *
 *    as_create - create a new empty address space. You need to make 
 *                sure this gets called in all the right places. You
 *                may find you want to change the argument list. May
 *                return NULL on out-of-memory error.
 *
 *    as_copy   - create a new address space that is an exact copy of
 *                an old one. Probably calls as_create to get a new
 *                empty address space and fill it in, but that's up to
 *                you.
 *
 *    as_activate - make the specified address space the one currently
 *                "seen" by the processor. Argument might be NULL, 
 *		  meaning "no particular address space".
 *
 *    as_destroy - dispose of an address space. You may need to change
 *                the way this works if implementing user-level threads.
 *
 *    as_define_region - set up a region of memory within the address
 *                space.
 *
 *    as_prepare_load - this is called before actually loading from an
 *                executable into the address space.
 *
 *    as_complete_load - this is called when loading from an executable
 *                is complete.
 *
 *    as_define_stack - set up the stack region in the address space.
 *                (Normally called *after* as_complete_load().) Hands
 *                back the initial stack pointer for the new process.
 */

struct addrspace *as_create(void);
int               as_copy(struct addrspace *src, struct addrspace **ret);
void              as_activate(struct addrspace *);
void              as_destroy(struct addrspace *);

int               as_define_region(struct addrspace *as, 
				   vaddr_t vaddr, size_t sz,
				   int readable, 
				   int writeable,
				   int executable);
int		  as_prepare_load(struct addrspace *as);
int		  as_complete_load(struct addrspace *as);
int               as_define_stack(struct addrspace *as, vaddr_t *initstackptr);

/* Helper functions to copy the page table */
bool copy_pages(struct secondlevel_pagetable *old, struct secondlevel_pagetable *new);
bool copy_pagetable(struct addrspace *old, struct addrspace *new);


/*
 * Functions in loadelf.c
 *    load_elf - load an ELF user program executable into the current
 *               address space. Returns the entry point (initial PC)
 *               in the space pointed to by ENTRYPOINT.
 */

int load_elf(struct vnode *v, vaddr_t *entrypoint);


#endif /* _ADDRSPACE_H_ */
