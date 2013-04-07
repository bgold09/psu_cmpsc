#ifndef _473_MM_H
#define _473_MM_H

/**
 * 'mm_init()' initializes the memory management system. 
 * 
 * @param vm: pointer to the start of virtual address space
 * @param vm_size: size of the virtual address space
 * @param n_frames: number of physical pages available in the system
 * @param page_size: size of both virtual and physical pages
 * @param policy: 1 indicates fifo replacement policy,
 *                2 indicates clock replacement policy
 * @return void
 */
void mm_init(void* vm, int vm_size, int n_frames, int page_size, int policy); 

/**
 * mm_report_npage_faults - return the total number of page faults of 
 *                          the entire system (across all virtual pages)
 *
 * @return total number of page faults
 */
unsigned long mm_report_npage_faults(); 

/**
 * mm_report_nwrite_backs - return the total number of write backs of 
 *                          the entire system (across all virtual pages)
 *
 * @return total number of write backs
 */
unsigned long mm_report_nwrite_backs();

#endif
