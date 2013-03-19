/**
 * @file my_memory.c
 * @brief implement memory allocation/deallocation schemes using
 *        First Fit, Best Fit, Worst Fit and Buddy System
 * @date 04/01/13
 * @author Mike Fitz, Brian Golden
 *
 */

#include <stdio.h>
#include <stdlib.h>     /* malloc(), free() */
#include "my_memory.h"

#define NUM_BUCKETS 11  /* number of block buckets needed for Buddy System */

static int type;                      /* memory allocation scheme to use */
free_list_t *free_list[NUM_BUCKETS];  /* free_lists for allocation */

void *my_malloc(int size)
{
	block_t *block;  /* pointer to a free block */
	void *ret;       /* pointer to newly allocated segment on the heap */

	/* find block to use for allocation and remove it from the free list */
	if ((block = find_and_remove_block(size + HEADER_SIZE)) == NULL) {
		return NULL;
	}
	
	ret = block->addr;
	*((int *) ret) = size;  /* add size of new block to its header */

	/* split the block, if necessary */
	if (size + HEADER_SIZE == block->size) {
		block_deallocate(block);
	} else {
		block->size -= size + HEADER_SIZE;
		block->addr += size + HEADER_SIZE;
		/* readd the block to the correct free_list */
		free_list_enqueue(block);
	}

	return ret + HEADER_SIZE;
}

block_t *find_and_remove_block(int size)
{
	switch(type) {
	case FF:
		return find_and_remove_block_ff(size);
		break;
	case BF:
		return find_and_remove_block_bf(size);
		break;
	case WF:
		return find_and_remove_block_wf(size);
		break;
	case BS:
		return find_and_remove_block_bs(size);
		break;
	default:
		/* default to First Fit allocation */
		return find_and_remove_block_ff(size);
		break;
	}
}

block_t *free_list_remove(free_list_t *list, block_t *block)
{
	block_t *p;  /* pointer to free block to remove from list */

	p = NULL;

	/* one element list, assuming non-null */
	if (list->head == list->tail) { 
		p = list->head;
		list->head = NULL;
		list->tail = NULL;
	} else if (list->head == block) {
		p = list->head;
		list->head = p->next;
		list->head->prev = NULL;
		p->next = NULL;
		p->prev = NULL;
	} else if (list->tail == block) {
		p = list->tail;
		list->tail = p->prev;
		list->tail->next = NULL;
		p->next = NULL;
		p->prev = NULL;
	} else {	
		p = block;
		p->prev->next = p->next;
		p->next->prev = p->prev;
		p->next = NULL;
		p->prev = NULL;
	}

	return p;
}

block_t *find_and_remove_block_ff(int size)
{
	free_list_t *list;  /* free list to remove from */
	block_t *p;         /* block removed from list */

	list = free_list[0];

	if (list->head->size >= size) {
		return free_list_remove(list, list->head);
	} 

	for (p = list->head->next; p != list->tail; p = p->next) {
		if (p->size >= size) {
			return free_list_remove(list, list->head);
		}
	}

	if (list->tail->size >= size) {
		return free_list_remove(list, list->tail);
	}
	
	return NULL;
}

block_t *find_and_remove_block_bf(int size)
{
	free_list_t *list;  /* free list to remove from */
	block_t *best;      /* best fit block for allocation */
	block_t *p;         /* temp block */
	int bestsize;       /* size of the current best fit block */

	list = free_list[0];
	bestsize = MAXRAMSIZE + 1;
	best = NULL;

	for (p = list->head; p != NULL; p = p->next) {
		if (p->size >= size && p->size < bestsize) {
			best = p;
			bestsize = p->size;			
		}
	}

	return best != NULL ? free_list_remove(list, best) : NULL;
}

block_t *find_and_remove_block_wf(int size)
{
	(void) size;
	return NULL;
}

block_t *find_and_remove_block_bs(int size)
{
	(void) size;
	return NULL;
}

void my_free(void *ptr)
{
	int size;
	block_t *block;
	free_list_t *list;

	size = *((int *) (ptr - HEADER_SIZE));
	block = block_allocate(ptr - HEADER_SIZE, size + HEADER_SIZE);
	list = free_list_enqueue(block);   /* add to correct free_list */
	coalesce(list, block);
}

void coalesce(free_list_t *list, block_t *ptr)
{
	if (type != BS) {
		if (ptr->next != NULL) {  /* not at tail of list */
			if (ptr->addr + ptr->size == ptr->next->addr) {
				ptr->size += ptr->next->size;
				free_list_remove(list, ptr->next); 
			}
		} 

		if (ptr->prev != NULL) {  /* not at head of the list */
			if (ptr->prev->addr + ptr->prev->size == ptr->addr) {
				ptr->prev->size += ptr->size;
				free_list_remove(list, ptr);
			}
		}
	} else {
		return;
	}
}

void setup(int malloc_type, int mem_size, void *start_of_memory)
{
	int i;

	type = malloc_type;

	if (malloc_type != BS) {
		free_list[0] = free_list_allocate();
		free_list[0]->head = block_allocate(start_of_memory, mem_size);
		free_list[0]->tail = free_list[0]->head;
	} else {
		for (i = 0; i < NUM_BUCKETS; i++) {
			free_list[i] = free_list_allocate();
			/* determine where to put the initial free segment */
		}
	}
}

block_t *block_allocate(void *addr, int size)
{
	block_t *p;   /* pointer to new free block to allocate */

	if ((p = malloc(sizeof(*p))) == NULL) {
		fprintf(stderr, "malloc() failed\n");
	} else {
		p->addr = addr;
		p->size = (size_t) size;
		p->prev = NULL;
		p->next = NULL;
	}

	return p;
}

void block_deallocate(block_t *p)
{
	free(p);
	p = NULL;
}

free_list_t *free_list_allocate(void)
{
	free_list_t *p;

	if ((p = (free_list_t *) malloc(sizeof(*p))) == NULL) {
		fprintf(stderr, "malloc failed, free_list_allocate()\n");
	}

	return p;
}

free_list_t *free_list_enqueue(block_t *block)
{
	free_list_t *list;
	block_t *p;

	list = NULL;

	if (type != BS) {
		list = free_list[0];

		if (list->head == NULL) {
			block->next = NULL;
			block->prev = NULL;
			list->head = block;
			list->tail = block;
			return list;
		}

		if (list->head->addr > block->addr) {
			p = list->head;
			list->head = block;
			list->head->next = p;
			list->head->prev = NULL;
			p->prev = list->head;
			return list;
		}

		for (p = list->head->next; 
		     p->addr < block->addr && p->next != NULL; p = p->next);

		if (p->next != NULL) {  /* not enqueuing at the tail */
			p->prev->next = block;
			block->prev = p->prev;
			block->next = p;
			p->prev = block;
		} else {                /* inserting after p, new tail */
			p->next = block;
			block->prev = p;
			block->next = NULL;
			list->tail = block;
		}
	} else {
		return list;
	}

	return list;
}
