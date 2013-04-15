#ifndef _473_MM_H
#define _473_MM_H

#include <signal.h>  /* siginfo_t */

#define FIFO  1      /* First-In-First-Out */
#define CLOCK 2      /* Clock Replacement  */
#define PB    0x4    /* present bit        */
#define RB    0x2    /* reference bit      */
#define MB    0x1    /* modify bit         */

/**
 * struct _entry_t - information about a virtual page, 
 *                   either in the page table or page history
 *
 */
struct _entry_t {
	int flags;  /* present, modify and reference bits */
	int index;  /* index in the page table */
	struct _entry_t *next;  /* pointer to next page in the history */
};
typedef struct _entry_t entry_t;

/**
 * struct _list_pages - singly-linked list of virtual pages
 *
 */
struct _list_pages {
	entry_t *head;  /* head of the list, NULL indicates empty list */
	entry_t *tail;  /* tail of the list */
};
typedef struct _list_pages list_pages;

typedef void sighandler_t(int, siginfo_t *, void *);

int type;         /* page replacement policy */
int faults;       /* number of page faults */
int write_backs;  /* number of write-backs on modified pages */
int table_size;   /* number of page table entries */
int max_frames;   /* number of physical frames available */
int cur_frames;   /* number of frames currently being used */
int page_size;    /* size of a page in the system */
void *vm_start;   /* base address of virtual memory */

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
void mm_init(void* vm, int vm_size, int n_frames, int pagesize, int policy); 

/**
 * mm_report_npage_faults - return the total number of page faults of 
 *                          the entire system (across all virtual pages)
 *
 * @return total number of page faults
 */
unsigned long mm_report_npage_faults(void); 

/**
 * mm_report_nwrite_backs - return the total number of write backs of 
 *                          the entire system (across all virtual pages)
 *
 * @return total number of write backs
 */
unsigned long mm_report_nwrite_backs(void);

/**
 * install_signal_handler - install a signal handler for this process
 *
 * @param sig: signal number to install a signal handler for
 * @param func: name of the signal handling function to use for this signal
 * @return int 0 success, -1 error
 */
int install_signal_handler(int sig, sighandler_t func);

/**
 * SIGSEGV_handler - signal handler function for SIGSEGV signals
 *
 * @param sig: SIGSEGV signal number
 * @param si: signal interrupt information
 * @param unused: unused parameter, must be included for 
 *                this form of a signal handler
 * @return void
 */
void SIGSEGV_handler(int sig, siginfo_t *si, void *unused);

/**
 * entry_allocate - allocate a new entry_t
 *
 * @param index: this virtual page's index in the page table
 * @return pointer to new entry_t on success, NULL failure
 */
entry_t *entry_allocate(int index);

/**
 * get_table_index - get the index in the page table for a memory address
 *
 * @param addr: address in memory
 * @return page number for addr
 */
int get_table_index(const void *addr);

/**
 * get_page_base - get the base address for a virtual page
 *
 * @param pagenum: index on the page table
 * @return address of the virtual page at index pagenum in the page table
 */
void *get_page_base(int pagenum);

/**
 * history_add - add a page to the history based on the 
 *               current page replacement scheme
 *
 * @param t: virtual page to add to the page history
 * @return void
 */
void history_add(entry_t *t);

/**
 * history_add_fifo - add a page to the history based on
 *                    First-In-First-Out page replacement 
 *
 * @param t: virtual page to add to the page history
 * @return void
 */
void history_add_fifo(entry_t *t);

/**
 * history_add_clock - add a page to the history based on
 *                     clock page replacement
 *
 * @param t: virtual page to add to the page history
 * @return void
 */
void history_add_clock(entry_t *t);

/**
 * evict_page - evict a page from physical memory based on
 *              the current page replacement scheme
 *
 * @return void
 */
void evict_page(void);

/**
 * evict_page_fifo - evict a page from physical memory based on
 *                   First-In-First-Out page replacement
 *
 * @return void
 */
void evict_page_fifo(void);

/**
 * evict_page_clock - evict a page from physical memory based on
 *                    clock page replacement scheme
 *
 * @return void
 */
void evict_page_clock(void);

#endif
