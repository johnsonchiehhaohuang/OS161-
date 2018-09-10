#ifndef _VM_H_
#define _VM_H_

#include <machine/vm.h>
#include <synch.h>
/*
 * VM system-related definitions.
 *
 * You'll probably want to add stuff here.
 */

/* Fault-type arguments to vm_fault() */
#define VM_FAULT_READ        0    /* A read was attempted */
#define VM_FAULT_WRITE       1    /* A write was attempted */
#define VM_FAULT_READONLY    2    /* A write to a readonly page was attempted*/

/* Initialization function */
void vm_bootstrap(void);
/* Coremap Initialization */
void cm_bootstrap(void);
/* Fault handling function called by trap code */
int vm_fault(int faulttype, vaddr_t faultaddress);
/* Function to print out available physical pages */
void print_coremap(void);
/* Allocate/free kernel heap pages (called by kmalloc/kfree) */
vaddr_t alloc_kpages(int npages);
paddr_t getppages(int npages);
void free_kpages(vaddr_t addr);
/* Helper functions for demand paging */
void free_coremap_entry(paddr_t p_addr);
bool on_stack(struct addrspace *as, vaddr_t faultaddress);
bool on_heap(struct addrspace *as, vaddr_t faultaddress);
bool find_PTE(struct addrspace *as, vaddr_t faultaddress, paddr_t *p_addr);
struct page_struct *get_physicalpage(void);
bool insert_PTE(struct addrspace *as, vaddr_t faultaddress, struct  page_struct *free_page);
bool increase_heap(struct addrspace *as, vaddr_t faultaddress, paddr_t *p_addr);
bool increase_stack(struct addrspace *as, vaddr_t faultaddress, paddr_t *p_addr);
bool load_page(struct addrspace *as, vaddr_t faultaddress, paddr_t *p_addr, int region);
bool as_load(struct addrspace *as, off_t offset, vaddr_t faultaddress, vaddr_t base, paddr_t paddr, size_t memsize, size_t filesize);



#endif /* _VM_H_ */
