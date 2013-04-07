#ifndef _473_MM_H
#define _473_MM_H

#include <signal.h>  /* siginfo_t */

#define FIFO  1      /* First-In-First-Out */
#define CLOCK 2      /* Clock Replacement  */
#define PB    0x4    /* present bit        */
#define RB    0x2    /* reference bit      */
#define MB    0x1    /* modify bit         */

struct _entry_t {
	int frame;
	int flags;
	struct _entry_t *next;
};
typedef struct _entry_t entry_t;

struct _list_pages {
	entry_t *head;
	entry_t *tail;
};
typedef struct _list_pages list_pages;

typedef void sighandler_t(int, siginfo_t *, void *);

int type;         /* page replacement policy */
int faults;
int write_backs;
int table_size;
int offset_bits;
int max_frames;
int cur_frames;

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

/**
 *
 *
 */
int install_signal_handler(int sig, sighandler_t func);

/**
 *
 */
void SIGSEGV_handler(int sig, siginfo_t *si, void *unused);

entry_t *entry_allocate(int frame);

#endif
