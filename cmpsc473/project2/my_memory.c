/**
 * @file my_memory.c
 * @brief implement memory allocation/deallocation schemes using
 *        First Fit, Best Fit, Worst Fit and Buddy System
 * @date 04/01/13
 * @author Mike Fitz, Brian Golden
 *
 */

#include <stdio.h>
#include <stdlib.h>  /* malloc(), free() */

int type;            /* memory allocation scheme to use */

void *my_malloc(int size)
{
	(void) size;
	return NULL;
}

void my_free(void *ptr)
{
	(void) ptr;
}

void setup(int malloc_type, int mem_size, void *start_of_memory)
{
	(void) malloc_type;
	(void) mem_size;
	(void) start_of_memory;
}
