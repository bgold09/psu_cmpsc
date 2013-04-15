#include <stdio.h>
#include <stdlib.h>    /* free() */
#include <unistd.h>    /*  */
#include <string.h>    /* strerror() */
#include <signal.h>    /* sigaction(), sigemptyset() */
#include <malloc.h>    /*  */
#include <math.h>      /* log2f() */
#include <errno.h>
#include <sys/mman.h>  /*  */
#include "473_mm.h"

static entry_t **page_table;   /*   */
static list_pages *page_hist;  /*   */

void mm_init(void* vm, int vm_size, int n_frames, int pagesize, int policy)
{
	int i;

	type = policy;
	faults = 0;
	write_backs = 0;
	table_size = vm_size / pagesize;
	max_frames = n_frames;
	cur_frames = 0;
	page_size = pagesize;
	vm_start = vm;

	if (install_signal_handler(SIGSEGV, SIGSEGV_handler) == -1) {
		fprintf(stderr, "install_signal_handler() failed: %s\n", 
				strerror(errno));
	} else if ((page_hist = (list_pages *) 
			malloc(sizeof(*page_hist))) == NULL) {
		fprintf(stderr, "malloc failed\n");
	} else if ((page_table = (entry_t **) 
			malloc(sizeof(*page_table) * table_size)) == NULL) {
		fprintf(stderr, "malloc failed\n");
	} else if (mprotect(vm, vm_size, PROT_NONE) == -1) {
		fprintf(stderr, "mprotect() failed: %s\n", strerror(errno));
	} else {
		for (i = 0; i < table_size; i++) {
			page_table[i] = entry_allocate(i);
		}

		page_hist->head = NULL;
		page_hist->tail = NULL;
	}
}

unsigned long mm_report_npage_faults(void)
{
	return faults;
}

unsigned long mm_report_nwrite_backs(void)
{
	return write_backs;
}

int install_signal_handler(int sig, sighandler_t func)
{
	int error;                /* true if installation failed */
	struct sigaction sigact;  /* signal handler descriptor */
	int ret;                  /* error indicator */

	sigact.sa_flags = SA_SIGINFO;

	/* do not mask any interrupts while in handler */
	sigemptyset(&sigact.sa_mask);         
	sigact.sa_sigaction = func;           /* name of sig handler function */

	ret = sigaction(sig, &sigact, NULL);  /* do the installation */
	error = (ret == -1);                  /* error condition */

	if (error) {
		fprintf(stderr, "install_signal_handler(%d) failed: %s\n", 
				sig, strerror(errno));
	}

	return (error ? -1 : 0);
}

void SIGSEGV_handler(int sig, siginfo_t *si, void *unused)
{
	int pagenum;      /* page number of the virtual page being accessed */
	void *virt_page;  /* address of the virtual page */

	(void) unused;	

	if (sig == SIGSEGV) {
		pagenum = get_table_index(si->si_addr);
		virt_page = get_page_base(pagenum);

		if (!(page_table[pagenum]->flags & PB)) { /* page not present */
			history_add(page_table[pagenum]);

			if (cur_frames == max_frames) { /* page table is full */
				/* evict according to page replacement */
				evict_page();
			} else {
				cur_frames++;
			}
			faults++;
			page_table[pagenum]->flags = PB | RB;

			/* set read permissions */
			if (mprotect(virt_page, page_size, PROT_READ) == -1) {
				fprintf(stderr, "mprotect failed: %s\n", 
						strerror(errno));
			}
		} else {   /* writing to a page for the first time */
			/* set modify bit */
			page_table[pagenum]->flags = PB | RB | MB; 

			/* set write permissions */
			if (mprotect(virt_page, page_size, 
						PROT_READ | PROT_WRITE) == -1) {
				fprintf(stderr, "mprotect failed: %s\n", 
						strerror(errno));
			}
		}
	} else {
		fprintf(stderr, "serious error, unknown signal received\n");
	}
}

entry_t *entry_allocate(int index)
{
	entry_t *p;

	if ((p = (entry_t *) malloc(sizeof(*p))) == NULL) {
		fprintf(stderr, "malloc() failed\n");
	}

	p->flags = 0;
	p->next = NULL;
	p->index = index;

	return p;
}

int get_table_index(const void *addr) 
{
	return (int) ((addr - vm_start) / (page_size));
}

void *get_page_base(int pagenum)
{
	return vm_start + pagenum * page_size;
}

void history_add(entry_t *t)
{
	if (type == FIFO) {
		history_add_fifo(t);
	} else if (type == CLOCK) {
		history_add_clock(t);
	} else {
		fprintf(stderr, "unknown page replacement type %d\n", type);
	}
}

void history_add_fifo(entry_t *t)
{
	if (page_hist->head == NULL) {  /* empty page history */
		page_hist->head = t;
		page_hist->tail = t;
	} else {
		page_hist->tail->next = t;
		page_hist->tail = t;
	}
	return;
}

void history_add_clock(entry_t *t)
{
	(void) t;
	return;
}

void evict_page(void)
{
	if (type == FIFO) {
		evict_page_fifo();
	} else if (type == CLOCK) {
		evict_page_clock();
	} else {
		fprintf(stderr, "unknown page replacement type %d\n", type);
	}
}

void evict_page_fifo(void)
{
	entry_t *head;    /* current head of the history, the page to evict */
	int pagenum;      /* page number of virtual page to evict */
	void *virt_page;  /* address of the virtual page to evict */

	head = page_hist->head;
	pagenum = head->index;

	if (head->flags & MB)  /* modify bit is set */
		write_backs++;
	
	head->flags = 0;
	page_hist->head = page_hist->head->next;
	head->next = NULL;

	virt_page = get_page_base(pagenum);

	/* unset permissions on the page */
	if (mprotect(virt_page, page_size, PROT_NONE) == -1) {
		fprintf(stderr, "mprotect failed: %s\n", strerror(errno));
	}

	return;
}

void evict_page_clock(void)
{
	return;
}
