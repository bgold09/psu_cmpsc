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

static entry_t **page_table;    /*   */
static list_pages *page_hist;  /*  */

void mm_init(void* vm, int vm_size, int n_frames, int page_size, int policy)
{
	int i;

	type = policy;
	faults = 0;
	write_backs = 0;
	table_size = vm_size / page_size;
	offset_bits = 64 - (int) log2f((int) table_size);
	max_frames = n_frames;
	cur_frames = 0;

	if (install_signal_handler(SIGSEGV, SIGSEGV_handler) == -1) {
		fprintf(stderr, "install_signal_handler() failed: %s\n", 
				strerror(errno));
	} else if ((page_hist = (list_pages *) malloc(sizeof(*page_hist))) == NULL) {
		fprintf(stderr, "malloc failed\n");
	} else if ((page_table = (entry_t **) malloc(sizeof(*page_table) * table_size)) == NULL) {
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
	return 0;
}

unsigned long mm_report_nwrite_backs(void)
{
	return 0;
}

int install_signal_handler(int sig, sighandler_t func)
{
	int error;                /* true if installation failed */
	struct sigaction sigact;  /* signal handler descriptor */
	int ret;                  /* error indicator */

	sigact.sa_flags = SA_SIGINFO;
	sigemptyset(&sigact.sa_mask);         /* do not mask any interrupts while in handler */
	sigact.sa_sigaction = func;           /* name of the signal handler function */

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
	(void) unused;
	(void) si;

	if (sig == SIGSEGV) {
		/* printf("got SIGSEV at address 0x%lx\n", (long) si->si_addr); */
		/* if page not present
		 * 	bring it into physical memory
		 * 	evict if necessary (num_frames == max_frames)
		 * 		if modify bit set, increment write backs
		 * 	increment faults
		 * 	set read permissions
		 * else 
		 * 	set modify bit
		 *	set read-write permissions
		 */

	} else {
		fprintf(stderr, "serious error, unknown signal received\n");
	}

	exit(EXIT_FAILURE);
}

entry_t *entry_allocate(int frame)
{
	entry_t *p;

	if ((p = (entry_t *) malloc(sizeof(*p))) == NULL) {
		fprintf(stderr, "malloc() failed\n");
	}

	p->frame = frame;
	p->flags = 0;

	return p;
}
